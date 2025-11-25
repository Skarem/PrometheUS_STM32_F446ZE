import os
import sys
import time
import argparse
import serial
import struct
import serial.tools.list_ports
import matplotlib.pyplot as plt
from collections import deque
from datetime import datetime

# ==================================================
# CONFIGURATION
# ==================================================

# ----- Serial -----
BAUDRATE = 921600

# ----- Packet -----
# Header
HEADER          = 0xABCD
ENDIANNESS      = "<"                                   # Little-endian
HEADER_FORMAT   = ENDIANNESS + "H"                      # H = uint16_t header
HEADER_BYTES    = struct.pack(HEADER_FORMAT, HEADER)

# Payload
FINGER_COUNT    = 3
PAYLOAD_LENGTH   = (FINGER_COUNT * 3) + 1                # 10 floats, 1 bool, 1 uint8_t
PAYLOAD_FORMAT  = "I" + ("f" * PAYLOAD_LENGTH) + "?B"    # I = uint32_t, f = float, ? = bool, I = uint8_t
PAYLOAD_SIZE    = struct.calcsize(PAYLOAD_FORMAT)

# Packet
PACKET_FORMAT   = HEADER_FORMAT + PAYLOAD_FORMAT
PACKET_SIZE     = struct.calcsize(PACKET_FORMAT)
UNPACK_PACKET   = struct.Struct(PACKET_FORMAT).unpack

# ----- Plot -----
WINDOW_SEC              = 5
RX_RATE_HZ              = 10
MAX_POINTS              = WINDOW_SEC * RX_RATE_HZ
UPDATE_EVERY_N_PACKETS  = 1
COLOR_LIST              = ['tab:red', 'tab:green', 'tab:blue']
COLOR_MOTOR             = 'tab:orange'

YLIM_POTS   = (0.0, 100.0)  # Percentage
YLIM_TEMP   = (10.0, 50.0)  # Celsius
YLIM_DUTY   = (0.45, 0.60)  # [0.0 - 1.0]
YLIM_MOTOR  = (0.0, 750.0) # RPM

# ----- Logging -----
LOG_DIR     = "logs"
PLOT_TITLE  = "Real-Time Telemetry Viewer"
LOG_HEADER  = (
    "time_ms;"
    "pot_1_deg;pot_2_deg;pot_3_deg;"
    "temp_1_C;temp_2_C;temp_3_C;"
    "duty_1_A;duty_2_A;duty_3_A;"
    "motor_RPM\n"
)


# ==================================================
# SERIAL SETUP
# ==================================================
def find_stm_port():
    """Find an STM32 serial port automatically."""
    for p in serial.tools.list_ports.comports():
        if "STM" in p.description or "STMicro" in p.description:
            return p.device
    return None


def open_serial():
    """Open the serial port in blocking mode."""
    port = find_stm_port()
    ser = serial.Serial(port, BAUDRATE, timeout = None)
    print(f"[INFO] Connected to {port}")
    return ser


# ==================================================
# LOGGING SETUP
# ==================================================
def open_log_file():
    """Open log file in log directory with file name."""
    os.makedirs(LOG_DIR, exist_ok = True)
    timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    filename = f"{LOG_DIR}/telemetry_{timestamp}.csv"
    log = open(filename, "w", buffering = 1)
    log.write(LOG_HEADER)
    print(f"[INFO] Logging to {filename}")
    return log


# ==================================================
# PACKET READER
# ==================================================
def read_packet_blocking(ser):
    """Wait for a valid packet (auto-resync if needed)."""
    buf = bytearray()
    # ----- Find header -----
    while True:
        b = ser.read(1)
        if not b:
            continue
        buf += b
        if len(buf) >= 2:
            if buf[-2:] == HEADER_BYTES:
                break
            buf = buf[-1:]

    # ----- Read rest of payload -----
    payload = ser.read(PAYLOAD_SIZE)
    if len(payload) != PAYLOAD_SIZE:
        return None
    
    data        = UNPACK_PACKET(HEADER_BYTES + payload)
    timestamp   = data[1]
    pots        = data[2:5]
    temp        = data[5:8]
    duty        = data[8:11]
    motor       = data[11]
    in_error    = data[12]
    error_src   = data[13]

    return timestamp / 1000.0, pots, temp, duty, motor, in_error, error_src


# ==================================================
# PLOT SETUP
# ==================================================
def setup_plot():
    """Setup plot for real-time viewing of data."""
    t_vals      = deque(maxlen  = MAX_POINTS)
    pots_vals   = [deque(maxlen = MAX_POINTS) for _ in range(FINGER_COUNT)]
    temp_vals   = [deque(maxlen = MAX_POINTS) for _ in range(FINGER_COUNT)]
    duty_vals   = [deque(maxlen = MAX_POINTS) for _ in range(FINGER_COUNT)]
    motor_vals  = deque(maxlen  = MAX_POINTS)

    fig, axes = plt.subplots(4, 1, sharex = True, figsize = (8, 6))
    ax_pots, ax_temp, ax_duty, ax_motor = axes
    for ax in axes:
        ax.grid(True, linestyle = '--', alpha = 0.5)

    lines_pots  = [ax_pots.plot([], [], color = COLOR_LIST[i], label = f"Pot  {i + 1}")[0] for i in range(FINGER_COUNT)]
    lines_temp  = [ax_temp.plot([], [], color = COLOR_LIST[i], label = f"Temp {i + 1}")[0] for i in range(FINGER_COUNT)]
    lines_duty  = [ax_duty.plot([], [], color = COLOR_LIST[i], label = f"Duty {i + 1}")[0] for i in range(FINGER_COUNT)]
    line_motor, = ax_motor.plot([], [], color = COLOR_MOTOR,   label = "Motor")

    ax_pots.set_ylabel("Potentiometers (deg)")
    ax_temp.set_ylabel("Clutch remperatures (C)")
    ax_duty.set_ylabel("Clutch duty cycles [0-1]")
    ax_motor.set_ylabel("Motor speed (RPM)")
    ax_motor.set_xlabel("Time (s)")

    ax_pots.set_ylim(*YLIM_POTS)
    ax_temp.set_ylim(*YLIM_TEMP)
    ax_duty.set_ylim(*YLIM_DUTY)
    ax_motor.set_ylim(*YLIM_MOTOR)

    for ax in axes:
        ax.legend(loc = 'upper right')

    plt.tight_layout()
    plt.ion()
    plt.show(block = False)
    fig.canvas.manager.set_window_title(PLOT_TITLE)

    return (fig, axes, lines_pots, lines_temp, lines_duty, line_motor,
            t_vals, pots_vals, temp_vals, duty_vals, motor_vals)


# ==================================================
# ERROR
# ==================================================

ERROR_TEXT_RED      = "\033[91m"
ERROR_TEXT_YELLOW   = "\033[93m"
ERROR_TEXT_RESET    = "\033[0m"

ERROR_SOURCES = {
    0: "No error",
    1: "Clutch temperature #1",
    2: "Clutch temperature #2",
    3: "Clutch temperature #3",
    4: "Motor error"
}

def describe_error(error_code, temp, motor):
    """
    Return human-readable description of the error,
    and include the corresponding measurement value.
    """
    msg = ERROR_SOURCES.get(error_code, f"Unknown error ({error_code})")

    # Match the description with the correct data value
    # Clutch temperatures
    if error_code in (1, 2, 3):
        idx = error_code - 1
        value = temp[idx] if 0 <= idx < len(temp) else None
        if value is not None:
            msg += f" - clutch temperature reading: {value:.2f} C"
    elif error_code == 4:
        msg += f" - motor velocity reading: {motor:.2f} RPM"

    return msg

# ==================================================
# PLOT UPDATE
# ==================================================
def update_plot(fig, axes, lines, t_vals, pots, temp, duty, motor):
    lines_pots, lines_temp, lines_duty, line_motor = lines
    if not t_vals:
        return
    
    for i in range(FINGER_COUNT):
        lines_pots[i].set_data(t_vals, pots[i])
        lines_temp[i].set_data(t_vals, temp[i])
        lines_duty[i].set_data(t_vals, duty[i])
    line_motor.set_data(t_vals, motor)

    x_min = t_vals[-1] - WINDOW_SEC
    x_max = t_vals[-1]
    for ax in axes:
        ax.set_xlim(x_min, x_max)
    
    plt.pause(0.001)


# ==================================================
# MAIN LOOP
# ==================================================
should_exit = False

def on_close(event):
    global should_exit
    print("\n[INFO] Plot window closed by user.")
    should_exit = True


def main(profile = False):
    ser = open_serial()
    log = open_log_file()
    (fig, axes, *rest) = setup_plot()
    lines_pots, lines_temp, lines_duty, line_motor, t_vals, pots_vals, temp_vals, duty_vals, motor_vals = rest

    fig.canvas.mpl_connect('close_event', on_close)

    packet_count = 0

    last_stm_time       = None
    last_real_time      = None
    desync_accumulator  = 0.0

    try:
        print("[INFO] Receiving telemetry... Ctrl+C to stop.")
        while not should_exit:
            # Read packet
            pkt = read_packet_blocking(ser)
            if not pkt:
                continue
            t, pots, temp, duty, motor, in_error, error_src = pkt

            # Log data
            log.write(f"{t:.3f};" + ";".join(f"{v:.3f}" for v in pots + temp + duty + (motor,)) + "\n")

            # Store in buffers
            t_vals.append(t)
            for i in range(FINGER_COUNT):
                pots_vals[i].append(pots[i])
                temp_vals[i].append(temp[i])
                duty_vals[i].append(duty[i])
            motor_vals.append(motor)

            if in_error:
                err_text = describe_error(error_src, temp, duty, motor)
                print(f"{ERROR_TEXT_RED}[ERROR]{ERROR_TEXT_RESET} {t:.3f}s")
                print(f"{ERROR_TEXT_YELLOW}{err_text}{ERROR_TEXT_RESET}")

            # Update plot at fixed interval
            if packet_count % UPDATE_EVERY_N_PACKETS == 0 or in_error:
                update_plot(fig, axes, (lines_pots, lines_temp, lines_duty, line_motor),
                            t_vals, pots_vals, temp_vals, duty_vals, motor_vals)
            
            now = time.time()

            # Sync tracking
            if last_stm_time is not None:
                dt_stm  = t - last_stm_time
                dt_real = now - last_real_time

                desync = dt_real - dt_stm
                desync_accumulator += desync

                if abs(desync_accumulator) > 0.5:   # 0.5s of cumulative lag
                    print(f"{ERROR_TEXT_YELLOW}[WARN]{ERROR_TEXT_RESET} Python falling behind by {desync_accumulator:+.2f}s")
                    desync_accumulator = 0.0

            last_stm_time   = t
            last_real_time  = now
            packet_count    += 1

    except KeyboardInterrupt:
        print("\n[INFO] User interrupted")
    finally:
        ser.close()
        log.close()
        print("[INFO] Serial and log closed")


# ==================================================
# ENTRY POINT
# ==================================================
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Real-time STM32 telemetry visualizer.")
    parser.add_argument("--profile", action="store_true", help="Enable packet rate profiling.")
    args = parser.parse_args()
    main(profile = args.profile)

# Importing Libraries
import serial
import matplotlib.pyplot as plt
import time

arduino = serial.Serial(port='COM3', baudrate=19200, timeout=.1)


def write_read():
    data = arduino.readline()
    return data


def main():
    plt.ion()
    fig, axs = plt.subplots(2)
    fig.suptitle('MPU data')
    axs[0].set_xlim([0, 100])
    axs[0].set_ylim([-300, 300])

    axs[1].set_xlim([0, 100])
    axs[1].set_ylim([-6, 6])

    curr_idx = 0

    gyr_x = [None] * 100
    gyr_y = [None] * 100
    gyr_z = [None] * 100

    acc_x = [None] * 100
    acc_y = [None] * 100
    acc_z = [None] * 100

    while True:
        l = []
        if curr_idx == 100:
            curr_idx = 0
            gyr_x = [None] * 100
            gyr_y = [None] * 100
            gyr_z = [None] * 100

            acc_x = [None] * 100
            acc_y = [None] * 100
            acc_z = [None] * 100
            l.clear()

            axs[0].clear()
            axs[1].clear()

            axs[0].set_xlim([0, 100])
            axs[0].set_ylim([-300, 300])

            axs[1].set_xlim([0, 100])
            axs[1].set_ylim([-6, 6])

        value = write_read()
        for t in value.split():
            try:
                l.append(float(t))
            except ValueError:
                pass
        if len(l) > 5:
            gyr_x[curr_idx] = (l[0])
            gyr_y[curr_idx] = (l[1])
            gyr_z[curr_idx] = (l[2])

            acc_x[curr_idx] = (l[3])
            acc_y[curr_idx] = (l[4])
            acc_z[curr_idx] = (l[5])
        print(l)

        line1, = axs[0].plot(range(0, len(gyr_x)), gyr_x, color='red', linewidth=1, label="X axis")
        line2, = axs[0].plot(range(0, len(gyr_y)), gyr_y, color='green', linewidth=1, label="Y axis")
        line3, = axs[0].plot(range(0, len(gyr_z)), gyr_z, color='blue', linewidth=1, label="Z axis")
        axs[0].legend(handles=[line1, line2, line3])
        axs[0].set_title("Gyroscope data")

        line4, = axs[1].plot(range(0, len(acc_x)), acc_x, color='red', linewidth=1, label="X axis")
        line5, = axs[1].plot(range(0, len(acc_y)), acc_y, color='green', linewidth=1, label="Y axis")
        line6, = axs[1].plot(range(0, len(acc_z)), acc_z, color='blue', linewidth=1, label="Z axis")
        axs[1].legend(handles=[line4, line5, line6])
        axs[1].set_title("Accelerometer data")

        line1.set_xdata(range(0, 100))
        line1.set_ydata(gyr_x)

        line2.set_xdata(range(0, 100))
        line2.set_ydata(gyr_y)

        line3.set_xdata(range(0, 100))
        line3.set_ydata(gyr_z)

        line4.set_xdata(range(0, 100))
        line4.set_ydata(acc_x)

        line5.set_xdata(range(0, 100))
        line5.set_ydata(acc_y)

        line6.set_xdata(range(0, 100))
        line6.set_ydata(acc_z)

        fig.canvas.draw()

        fig.canvas.flush_events()
        curr_idx = curr_idx + 1


if __name__ == "__main__":
    main()


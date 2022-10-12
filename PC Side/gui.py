import tkinter as tk
import tkinter.ttk as tkk

def manual_control():
    # Toplevel object which will
    # be treated as a new window
    manualControlWindow = tk.Toplevel(mainWindow)

    manualControlWindow.title("Manual Control")
    manualControlWindow.geometry("200x200")

    # A Label widget to show in toplevel
    tk.Label(manualControlWindow,
          text="Manual Control").pack()

def joystick_painter():
    # Toplevel object which will
    # be treated as a new window
    joystickPainterWindow = tk.Toplevel(mainWindow)

    joystickPainterWindow.title("Joystick Painter")
    joystickPainterWindow.geometry("200x200")

    # A Label widget to show in toplevel
    tk.Label(joystickPainterWindow,
          text="Joystick Painter").pack()

def stepper_motor_calibration():
    # Toplevel object which will
    # be treated as a new window
    stepperMotorCalibrationWindow = tk.Toplevel(mainWindow)

    stepperMotorCalibrationWindow.title("Stepper Motor Calibration")
    stepperMotorCalibrationWindow.geometry("200x200")

    # A Label widget to show in toplevel
    tk.Label(stepperMotorCalibrationWindow,
          text="Stepper Motor Calibration").pack()

def script_mode():
    # Toplevel object which will
    # be treated as a new window
    ScriptModeWindow = tk.Toplevel(mainWindow)
    ScriptModeWindow.title("Script Mode")
    ScriptModeWindow.geometry("200x200")

    # A Label widget to show in toplevel
    tk.Label(ScriptModeWindow, text="Script Mode").pack()
    ScriptModeWindow.rowconfigure([0, 1], minsize=100, weight=1)
    ScriptModeWindow.columnconfigure([0, 1, 2], minsize=100, weight=1)

    buttonLoad1 = tk.Button(master=ScriptModeWindow, text="Load File 1", width=20, height=5)
    buttonLoad1.grid(row=0, column=0)
    buttonLoad2 = tk.Button(master=ScriptModeWindow, text="Load File 2", width=20, height=5)
    buttonLoad2.grid(row=0, column=1)
    buttonLoad3 = tk.Button(master=ScriptModeWindow, text="Load File 3", width=20, height=5)
    buttonLoad3.grid(row=0, column=2)
    # buttonLoad1.pack()
    # buttonLoad2.pack()
    # buttonLoad3.pack()

    ScriptModeWindow.mainloop()

mainWindow = tk.Tk()
mainWindow.geometry('350x350')
mainWindow.title('DCS Project')
mainWindow.rowconfigure([0, 1, 2, 3], minsize=100, weight=1)
mainWindow.columnconfigure([0, 1], minsize=100, weight=1)

mainFrame = tk.Frame(master=mainWindow)
mainFrame.grid(row=0, columnspan=2)
mainLabel = tk.Label(master=mainFrame, text='DCS Project', font=('cambria', 24), width=100)
namesLabel = tk.Label(master=mainFrame, text='Almog Stern & Royi Zalk', font=('cambria', 18), width=100)



button1 = tk.Button(master=mainWindow, text='Manual Control', width=20, height=5,
                    command=manual_control)
button1.grid(row=1, column=0)
button2 = tk.Button(master=mainWindow, text='Joystick Painter', width=20, height=5,
                    command=joystick_painter)
button2.grid(row=1, column=1)
button3 = tk.Button(master=mainWindow, text='Stepper Motor Calibration', width=20, height=5,
                    command=stepper_motor_calibration)
button3.grid(row=2, column=0)
button4 = tk.Button(master=mainWindow, text='Script Mode', width=20, height=5,
                    command=script_mode)
button4.grid(row=2, column=1)

mainLabel.pack()
namesLabel.pack()


mainWindow.mainloop()

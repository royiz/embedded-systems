import serial as ser
import time
import paint
import os.path
#in this example we send a character, and receive a string in response only if the start character was 'u'

def main():
    s = ser.Serial('COM8', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)   # timeout of 1 sec so that the read and write operations are blocking,
                                # after the timeout the program continues

    menu_str = """1. Manual control of motor based machine
2. Joystick based PC painter
3. go to zero
4. calibartion mode - find the degree
5. start motor
6. stop motor
7. Script mode write
8. flash excute
9. Sleep"""
    """
    state1 :  motor1
    state2:  paint 2
    state 3: calibration 3,5,6,4,5,6
    state 4  flash 7, 8
    """


    print(menu_str)
    enableTX = True
    # clear buffers
    s.reset_input_buffer()
    s.reset_output_buffer()
    flag_in_zero=0
    calib_mode=0
    calib_data=0
    number_of_seg=0
    quite_paint=1
###

###
    #
    while 1:
        inChar= input("Enter state:")
        bytesChar = bytes(inChar, 'ascii')
        if inChar == "1": # joystick painter
            s.write(bytesChar)
            # while(True):
            #     while (s.in_waiting == 0):  # while the input buffer is empty
            #         pass
            #     angle=(s.read(size=1))
            #     int_angle = int.from_bytes(angle, "big")
            #     print (int_angle)


        if inChar == "2": # joystick painter
            s.write(bytesChar)
            paint.init_turtle()
            while quite_paint:
                while (s.in_waiting == 0):  # while the input buffer is empty
                    pass
                angle=(s.read(size=1))
                int_angle=int.from_bytes(angle,"big")
                if (int_angle==9):
                    break
                    inChar = input("for stay press 0 else press 1")
                    bytesChar = bytes(inChar, 'ascii')
                    s.write(bytesChar)
                if int_angle==9: # PB
                    time.sleep(1)
                if int_angle == 'q':
                    break
                if int_angle == 'b':
                    paint.close_screen()
                if int_angle == 'r':
                    paint.reset_screen()
                paint.handle_command(int_angle)


        if inChar == '3' : # the arrow go to 0
            s.write(bytesChar)
            calib_mode=1
            print("Press 5 for Start, or 6 for Stop")


        if inChar == '4' :   # all the loop
            s.write(bytesChar)
            calib_data=1;
            print("Press 5 for Start, or 6 for Stop")
            continue

            calib_mode = 1
        if inChar == '5' and calib_mode==1:     #start
            s.write(bytesChar)
        if inChar == '6' and calib_mode== 1:    #stop
            s.write(bytesChar)
            if (calib_data==1):
                while (s.in_waiting == 0):  # while the input buffer is empty
                    pass
                p=(s.read(size=1))           # P
                p=int.from_bytes(p, "big")
                while (s.in_waiting == 0):  # while the input buffer is empty
                    pass
                Q= (s.read(size=1))  # Q
                Q = int.from_bytes(Q, "big")
                number_steps=p*100+Q
                Phi=360/number_steps
                print ("number of steps: " +str(number_steps))
                print ("Phi: "+ str(Phi))
                calib_data=0
        if inChar == '7':                       #flash mode_write
            s.write(bytesChar)
            File_path_1 = "G:\תיקיות אחסון שיתופי\DCS רועי ואלמוג\Project\Scripts\script1.txt"
            File_path_2 = "G:\תיקיות אחסון שיתופי\DCS רועי ואלמוג\Project\Scripts\script2.txt"
            File_path_3 = "G:\תיקיות אחסון שיתופי\DCS רועי ואלמוג\Project\Scripts\script3.txt"
            files_paths=[File_path_1,File_path_2,File_path_3]
            for path in files_paths:
                filestr = file_to_str(path)
                print(filestr)
                for i in range(0, len(filestr)-2, 2):
                    bytesChar = int(filestr[i:i+2], base=16).to_bytes(1,'big')
                    time.sleep(0.25)
                    s.write(bytesChar)

                if(filestr[len(filestr)-1]=='~'):
                    bytesChar = bytes('~', 'ascii')
                    time.sleep(0.35)
                    s.write(bytesChar)

                else:
                    print (" error in sanding file")
                print("uplod first file")
            # for debug. remove!!
            # bytesChar = bytes('\n', 'ascii')
            # s.write(bytesChar)
            # s.write(bytesChar)
            # s.write(bytesChar)
            # print (len(filestr))
        if inChar == '8':  # flash execute
            s.write(bytesChar)
            seg_num = input("seg number  to execute (0-2)")
            bytes_seg_num = bytes(seg_num, 'ascii')
            s.write(bytes_seg_num)



            # if number_of_seg<3:
            #     #write to flash
            #     pathfile=input("Enter path:")
            #     filestr = file_to_str(pathfile)
            #     for i in range(0, len(filestr) - 2, 2):
            #         bytesChar = int(filestr[i:i+2], base=16)
            #         bytesChar=bytesChar.to_bytes(1,'big')
            #         s.write(bytesChar)
            #         # comunication option 1
            #             # while (s.in_waiting == 0):  # while the input buffer is empty
            #             #     pass
            #             # while (s.read(size=1)!='1'): # while the controller didnt indicate recive
            #             #     pass
            #         # delay option 2
            #         time.sleep(0.25)
            #     number_of_seg += 1
            #     print ( " the file in the controller")
            # else:
            #     print (" you have already 3 segment in the flash ")









def file_to_str(path):
    file = open(path, "r")
    temp = file.read().splitlines()
    str1 = ''.join(temp)
    # Length_string=len(str1)
    # str1=str(Length_string)+str1;
    str1 = (str1 + '~')
    return str1





if __name__ == '__main__':
    main()







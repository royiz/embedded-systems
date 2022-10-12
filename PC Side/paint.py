import turtle
import turtle as tu


_screen = None
_state = None


def init_turtle():
    global _screen, _state
    _screen = tu.getscreen()
    tu.title("Almog & Roey's DCS Project")
    _state = "p"
    tu.pendown()
    tu.mode("logo")


def reset_screen():
    tu.reset()


def close_screen():
    tu.bye()


def handle_command(command_letter):
    global _state
    if command_letter == 9: #C is for click on the button, not cookie
        if _state == "e":
            print("Changing to neutral mode")
            _state = "n"
            tu.penup()
        elif _state == "p":
            print("Changing to erase mode")
            _state = "e"
            tu.pendown()
            turtle.pencolor("white")
        elif _state == "n":
            print("Changing to paint mode")
            _state = "p"
            tu.pendown()
            turtle.pencolor("black")
    elif 0 <=command_letter<=8:
        command_int = int(command_letter)
        step_size = 2
        if 0 < command_int <= 8:
            tu.setheading((command_int-1) * 45)
            tu.forward(step_size)


if __name__ == "__main__":
    init_turtle()
    while True:
        print('Enter a command and give me a cookie:')
        c = input()
        if c=='q':
            break
        if c=='b':
            close_screen()
        if c=='r':
            reset_screen()
        handle_command(c)
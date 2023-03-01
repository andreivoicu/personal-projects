# initializing home window®
import Play
import Settings

from tkinter import *
from tkinter import font

from PIL import Image, ImageTk
from tkinter.ttk import *


def main():
    # creates a Tk() object
    master = Tk()
    master.title('speed typer')
    master.geometry("800x600")

    helv36 = font.Font(family='Comic', size=36, weight='bold')  # font
    bg = ImageTk.PhotoImage(file="home menu background.png")  # background for homePage

    canvas1 = Canvas(master, width=800, height=600)
    canvas1.pack(fill="both", expand=True)
    canvas1.create_image(0, 0, image=bg, anchor="nw")

    # Create style Object
    style = Style()
    style.configure('TButton', font=('Comic', 20, 'bold'))

    # Changes will be reflected
    # by the movement of mouse.
    style.map('TButton', foreground=[('active', '!disabled', 'green')], background=[('active', 'black')])

    # Buttons

    playButton = Button(master, text="Play")
    playButton.bind("<Button>", lambda e: [master.withdraw(), Play.PlayWindow(master)])
    playButton.place(relx=0.5, rely=0.25, anchor=N, height=70, width=170)

    settingsButton = Button(master, text="Settings")
    settingsButton.bind("<Button>", lambda e: [master.withdraw(), Settings.SettingsWindow(master)])
    settingsButton.place(relx=0.5, rely=0.5, anchor=CENTER, height=70, width=170)

    quitButton = Button(master, text="Quit", command=master.quit)
    quitButton.place(relx=0.5, rely=0.75, anchor=S, height=70, width=170)

    # mainloop, runs infinitely
    mainloop()


if __name__ == "__main__":
    main()

import Play

from tkinter import *
from tkinter import font

from PIL import Image, ImageTk
from tkinter.ttk import *

class HomeWindow(Toplevel):

    def __init__(self):

        self.title("Play!")
        self.geometry("800x600")
        label = Label(self, text="Play!")
        label.pack()

        helv36 = font.Font(family='Comic', size=36, weight='bold')  # font

        # Create style Object
        style = Style()
        style.configure('TButton', font=('Comic', 20, 'bold'))

        # Changes will be reflected
        # by the movement of mouse.
        style.map('TButton', foreground=[('active', '!disabled', 'green')], background=[('active', 'black')])

        # Buttons

        playButton = Button(self, text="Play")
        playButton.bind("<Button>", lambda e: [self.withdraw(), Play.PlayWindow(self)])
        playButton.place(relx=0.5, rely=0.25, anchor=N, height=70, width=170)

        settingsButton = Button(self, text="Settings")
        settingsButton.place(relx=0.5, rely=0.5, anchor=CENTER, height=70, width=170)

        quitButton = Button(self, text="Quit", command=self.quit)
        quitButton.place(relx=0.5, rely=0.75, anchor=S, height=70, width=170)

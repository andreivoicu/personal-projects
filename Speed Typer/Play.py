import random
import words
from tkinter import *
from tkinter import messagebox
import GlobalVariables as var
from PIL import Image, ImageTk

currentScore = 0
currentMissed = 0
currentTime = 0
currentDifficulty = "Normal"
wordList = []
characterNo = 0

class PlayWindow(Toplevel):

    def __init__(self, master):
        super().__init__(master=master)
        self.title("Play!")
        self.geometry("800x600")
        bg = ImageTk.PhotoImage(file="home menu background.png")  # background for homePage

        self.canvas = Canvas(master, width=800, height=600)
        self.background_image = ImageTk.PhotoImage(file="home menu background.png")
        self.background_label = Label(self, image=self.background_image)
        self.background_label.place(relwidth=1, relheight=1, anchor='nw')
        
    
       
        backButton = Button(self, text="<--")
        backButton.bind("<Button>", lambda e: [self.destroy(), master.update(), master.deiconify()])
        backButton.place(relx=0.05, rely=0.05, anchor=NW, height=30, width=80)

        global currentTime, currentScore, currentMissed, currentDifficulty, wordList, characterNo
        currentTime = var.getTime()
        currentScore = var.getScore()
        currentMissed = var.getMissed()
        currentDifficulty = var.getDifficulty()

        if currentDifficulty == "Easy":
            wordList = words.getEasy()
        elif currentDifficulty == "Medium":
            wordList = words.getMedium()
        elif currentDifficulty == "Hard":
            wordList = words.getHard()
        timercount = Label(self, font=('arial', 25, 'italic bold'), fg='blue')
        scorelabel = Label(self, font=('arial', 25, 'italic bold'), fg='red')
        scorelabelcount = Label(self, font=('arial', 25, 'italic bold'), fg='blue')
        labelfortimer = Label(self, font=('arial', 25, 'italic bold'), fg='red')
        gameinstruction = Label(self, text='Hit enter button after typing the word',font=('arial', 25, 'italic bold'), fg='grey')
        def giventime():
            timercount.place(x=665, y=100)
            scorelabel.place(x=50, y=100)
            scorelabelcount.place(x=250, y=100)
            labelfortimer.place(x=500, y=100)
            gameinstruction.place(relx= 0.5, rely=0.90, anchor=CENTER)
            global currentScore, currentMissed, currentTime, characterNo
            if currentTime > 11:
                pass
            else:
                timercount.configure(fg='red')
            if currentTime > 0:
                currentTime -= 1
                timercount.configure(text=currentTime)
                timercount.after(1000, giventime)
            else:
                gameinstruction.configure(text='Hit = {} | Miss = {} | Total score = {} | CPS = {}'
                                          .format(currentScore, currentMissed, currentScore - currentMissed, characterNo // var.getTime()))
                rr = messagebox.askretrycancel('Notification', 'Do you want to play again?')
                currentScore = 0
                currentMissed = 0
                characterNo = 0
                currentTime = var.getTime()
                if rr:
                    timercount.configure(text=currentTime)
                    labelforward.configure(text=wordList[0])
                    scorelabelcount.configure(text=currentScore)
                    gameinstruction.configure(text='Hit enter button after typing the word',font=('arial', 25, 'italic bold'), fg='grey')
                    wordentry.delete(0, END)
                else:
                    self.destroy()
                    master.update()
                    master.deiconify()

        def game(event):
            global currentScore, currentMissed, characterNo
            if currentTime == var.getTime():
                giventime()
            gameinstruction.configure()
            scorelabel.configure(text = "Your Score:")
            scorelabelcount.configure(text = currentScore)
            labelfortimer.configure(text = "Time Left:")
            timercount.configure(text = currentTime)
            startlabel.destroy()

            inputWord = wordentry.get()
            characterNo += len(inputWord)
            if inputWord == labelforward['text']:
                currentScore += 1
                scorelabelcount.configure(text=currentScore)
            else:
                currentMissed += 1
            random.shuffle(wordList)
            labelforward.configure(text=wordList[0])
            wordentry.delete(0, END)

        startlabel = Label(self, text='Start Typing', font=('arial', 45, 'italic bold'), fg='black')
        startlabel.place(relx=0.5, rely=0.25, anchor=CENTER)

        random.shuffle(wordList)
        labelforward = Label(self, text=wordList[0], font=('arial', 45, 'italic bold'), fg='green')
        labelforward.place(relx=0.5, rely = 0.40, anchor=CENTER)

        timercount = Label(self, font=('arial', 25, 'italic bold'), fg='blue')


        wordentry = Entry(self, font=('arial', 25, 'italic bold'), bd=10, justify='center')
        wordentry.place(relx=0.5, rely=0.55, anchor=CENTER)
        wordentry.focus_set()

        self.bind('<Return>', game)

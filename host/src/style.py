import hid
import screen

def display_percent_bar(screen, cloum, lable, percent):
    screen.cursor(0, cloum)
    width = screen.width()
    #clean screen
    screen.rectangle(0, cloum, width, 0)
    #format
    bar = width - (len(lable) + 2) * 6
    if percent < 0:
        percent = 0
    elif percent > 100:
        percent = 100
    fill = int(percent * bar / 100)
    screen.write(lable)
    screen.cursor(len(lable)*6+bar,cloum)
    if percent == 100:
        screen.write('**')
    else:
        screen.write('%02d'%percent)
    screen.rectangle(len(lable)*6, cloum, fill, 0x7E)


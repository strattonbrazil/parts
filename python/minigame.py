import sys

def pointContainsRect(mousePos, rect):
    mouseX, mouseY = mousePos
    rectX, rectY = rect["position"]
    rectWidth, rectHeight = rect["size"]
    return rectX < mouseX and rectY < mouseY and mouseX < rectX + rectWidth and mouseY < rectY + rectHeight

def scaleColor(color, scale):
    return tuple(map(lambda channel: min(channel * scale, 1), color))

def update_game(ctx):
    if "state" not in ctx:
        ctx["state"] = {
            "starting" : True
        }
    if "wasUp" not in ctx:
        ctx["wasUp"] = True

    unit = 1 / 7.0

    if ctx["mousePos"][0] > 0.5:
        color = [1,0,0]
    else:
        color = [0,1,0]
    assets = []
    colors = [(1,1,0), (1,0,1), (1,0,0), (0,1,1)]
    possibleSounds = ["piano-bb", "piano-c", "piano-eb", "piano-g"]
    possibleSound = None
    for i in range(4):
        row = i / 2
        column = i % 2

        rect = {
            "type" : "rectangle",
            "position" : [unit+unit*3*column, unit+unit*3*row],
            "size" : [unit*2, unit*2]
        }
        if pointContainsRect(ctx["mousePos"], rect):
            rect["color"] = colors[i]
            possibleSound = possibleSounds[i]
        else:
            rect["color"] = scaleColor(colors[i], 0.4)
        assets.append(rect)
    ctx["assets"] = assets

    if ctx["mouseDown"] and ctx["wasUp"]: # press
        if possibleSound:
            ctx["sound"] = possibleSound

        ctx["wasUp"] = False

    ctx["wasUp"] = not ctx["mouseDown"]

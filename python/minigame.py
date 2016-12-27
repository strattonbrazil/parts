import sys

def pointContainsRect(mousePos, rect):
    mouseX, mouseY = mousePos
    rectX, rectY = rect["position"]
    rectWidth, rectHeight = rect["size"]
    return rectX < mouseX and rectY < mouseY and mouseX < rectX + rectWidth and mouseY < rectY + rectHeight

def update_game(ctx):
    unit = 1 / 7.0

    if ctx["mousePos"][0] > 0.5:
        color = [1,0,0]
    else:
        color = [0,1,0]
    assets = []
    for row in range(2):
        for column in range(2):
            rect = {
                "type" : "rectangle",
                "position" : [unit+unit*3*column, unit+unit*3*row],
                "size" : [unit*2, unit*2]
            }
            if pointContainsRect(ctx["mousePos"], rect):
                rect["color"] = [1,0,0]
            else:
                rect["color"] = [0,1,0]
            assets.append(rect)
    ctx["assets"] = assets
    if ctx["mouseDown"]:
        ctx["finished"] = True


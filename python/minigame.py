import sys

def get_minigame_scene(gameContext):
    gameContext["assets"] = [
        {
            "type" : "rectangle",
            "color" : [0,1,0],
            "position" : [0.2,0.2],
            "size" : [0.5,0.5]
        }
    ]
    return gameContext

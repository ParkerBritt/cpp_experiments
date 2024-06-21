# rez package description
name = "pls"

version = "0.0.1"

authors = [
    "parker"
]

description = \
    """
    Custom implementation of LS command
    """

tools = [
    "pls"
]

requires = [
]

uuid = "03722972ecca45818a2f7540ff623317"

build_command = "bash {root}/build.sh"

def commands():
    env.PATH.append("{root}/build/bin")


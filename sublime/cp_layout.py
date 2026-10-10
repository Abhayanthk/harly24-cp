"""CP: Contest layout. Code on the left; input.txt top right; output, debug, info and error tabs bottom right."""

import os

import sublime
import sublime_plugin

LAYOUT = {
    "cols": [0.0, 0.54, 1.0],
    "rows": [0.0, 0.48, 1.0],
    "cells": [[0, 0, 1, 2], [1, 0, 2, 1], [1, 1, 2, 2]],
}
TOP_RIGHT = ["input.txt"]
BOTTOM_RIGHT = ["output.txt", "debug.txt", "info.txt", "error.txt"]


class CpLayoutCommand(sublime_plugin.WindowCommand):
    def run(self):
        code = self.window.active_view()
        if code is None or not code.file_name():
            sublime.status_message("CP layout: open the .cpp file you're solving first")
            return
        folder = os.path.dirname(code.file_name())

        self.window.set_layout(LAYOUT)
        self.move(code, 0)
        for name in TOP_RIGHT:
            self.place(os.path.join(folder, name), 1)
        for name in BOTTOM_RIGHT:
            self.place(os.path.join(folder, name), 2)
        self.window.focus_view(code)

    def place(self, path, group):
        if not os.path.exists(path):
            open(path, "a").close()   # cprun fills these on the first run
        view = self.window.find_open_file(path)
        if view is None:
            self.window.focus_group(group)
            view = self.window.open_file(path)
        self.move(view, group)

    def move(self, view, group):
        if self.window.get_view_index(view)[0] != group:
            self.window.set_view_index(view, group, len(self.window.views_in_group(group)))

# -*- coding: utf-8 -*-
""" python-part of PygmentsHighlighter

code/ideas taken from IPython, but IPython doesn't work with PythonQt...

Authors:
    "Melven Zoellner" <melven@topen.org>
"""


# allow to import pygments-stuff from current path
import sys


# adjust sys.path
dot_in_path = '.' in sys.path
if not dot_in_path:
    sys.path.insert(0, '.')

# Third-party imports
from pygments.lexers import PythonLexer
from pygments.formatters import HtmlFormatter

# PythonQt imports
from PythonQt import QtGui


# undo sys.path modifications
if not dot_in_path:
    sys.path.remove('.')


class PygmentsHighlighter(object):
    """ highlight python code with a QSyntaxHighlighter,
        callable class (e.g. function with a state) to """

    def __init__(self):
        """ constructor """
        self._lexer = PythonLexer()
        self._formatter = HtmlFormatter()
        self._document = QtGui.QTextDocument()
        self._document.setDefaultStyleSheet(self._formatter.get_style_defs())
        self._format_cache = dict()

    def __call__(self, code):
        """ makes this class callable, actually do the highlightning """
        index = 0
        for token, text in self._lexer.get_tokens(code):
            length = len(text)
            char_format = self._get_format(token)
            pygmentsHighlighter._setFormat(index, length, char_format)
            index += length

    def _get_format(self, token):
        """ get the QTextCharFormat for a token """
        if token in self._format_cache:
            return self._format_cache[token]

        # get format from document
        code, html = next(self._formatter._format_lines([(token, u'dummy')]))
        self._document.setHtml(html)
        char_format = QtGui.QTextCursor(self._document).charFormat()

        # cache result
        self._format_cache[token] = char_format

        return char_format


# create class instance, can be called with highlightCode(code)
highlightCode = PygmentsHighlighter()

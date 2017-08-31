# -*- coding: utf-8 -*-
""" python-part of PythonCompleter

code/ideas taken from IPython, but IPython doesn't work with PythonQt...

Authors:
    "Melven Zoellner" <melven@topen.org>
"""


# system imports
import sys
import rlcompleter


# adjust sys.path
dot_in_path = '.' in sys.path
if not dot_in_path:
    sys.path.insert(0, '.')

# Third-party imports
from pygments.lexers import PythonLexer
from pygments.token import Token, is_token_subtype

# local imports
from module_completion import module_completion


# undo sys.path modifications
if not dot_in_path:
    sys.path.remove('.')


# ugly fix to make rlcompleter work with PythonQt:
# problem:
#   hasattr raises SystemError for
#   <type 'PythonQt.PythonQtInstanceWrapper'>
#   this is probably a PythonQt-problem and should be fixed there?
def _get_class_members(klass):
    ret = dir(klass)
    try:
        for base in klass.__bases__:
            ret = ret + _get_class_members(base)
    except SystemError:
        pass
    return ret

rlcompleter.get_class_members = _get_class_members


def pythonqt_specific_completions(context, parent):
    """ determines pythonqt-specific completions,
        currently it adds only named children of QObjects """
    try:
        completions = list()
        # for completions for instances of PythonQt-Wrapper classes
        # we need to do some ugly stuff here
        # 1. get the base name
        base_name = '.'.join(context[0:-1])
        # 2. eval the name in the parent context
        base_object = eval(base_name, parent.__dict__)
        # 2. check if it is really an instance of a PythonQt-Wrapper class
        if repr(type(type(base_object))) == "<type 'PythonQt.PythonQtClassWrapper'>":
            # 3. look for the names of it's children
            if(hasattr(base_object, 'children') and callable(base_object.children)):
                for child in base_object.children():
                    if(hasattr(child, 'objectName') and child.objectName != ''):
                        completions.append(child.objectName)
        return completions
    except:
        return list()


def autocompleteCode(code, parent=sys.modules['__main__']):
    """ try to find useful completions for code """

    # get last line and context
    all_lines = code.split('\n')
    line = all_lines[-1]
    context = get_context(line)
    word = '.'.join(context)

    completions = None

    # module completion
    if line.startswith('from ') or line.startswith('import '):
        completions = module_completion(line)

    # rlcompleter
    if completions is None:
        completer = rlcompleter.Completer(parent.__dict__)
        if '.' in word:
            completions = completer.attr_matches(word)

            # also append PythonQt-specific completions
            completions += pythonqt_specific_completions(context, parent)

        else:
            completions = completer.global_matches(word)

    # we only need the last part of the completion
    # (e.g. my_function from my_class.my_function)
    completions = [c.split('.')[-1] for c in completions]

    # sort completions and remove duplicates
    completions = sorted(set(completions), key=(lambda s: s.lower()))

    # hide all members that begin with '_',
    # except the user starts a name with '_'
    if not (context and context[-1].startswith('_')):
        completions = [c for c in completions if not c.startswith('_')]

    # return results
    return completions


def get_context(string):
    """ Assuming the cursor is at the end of the specified string, get the
        context (a list of names) for the symbol at cursor position.
    """
    lexer = PythonLexer()
    context = []
    reversed_tokens = list(lexer.get_tokens(string))
    reversed_tokens.reverse()

    # Pygments often tacks on a newline when none is specified in the input.
    # Remove this newline.
    if reversed_tokens and reversed_tokens[0][1].endswith('\n') and \
            not string.endswith('\n'):
        reversed_tokens.pop(0)

    current_op = ''
    for token, text in reversed_tokens:

        if is_token_subtype(token, Token.Name):

            # Handle a trailing separator, e.g 'foo.bar.'
            if current_op == '.':
                if not context:
                    context.insert(0, '')

            # Handle non-separator operators and punction.
            elif current_op:
                break

            context.insert(0, text)
            current_op = ''

        # Pygments doesn't understand that, e.g., '->' is a single operator
        # in C++. This is why we have to build up an operator from
        # potentially several tokens.
        elif token is Token.Operator or token is Token.Punctuation:
            current_op = text + current_op

        # Break on anything that is not a Operator, Punctuation, or Name.
        else:
            break

    return context

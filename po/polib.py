#!/usr/bin/env python
# -*- coding: latin-1 -*-
#
# License: MIT (see LICENSE file provided)

### Description {{{
"""
A library to manipulate gettext catalogs files (aka .po and .mo files).

polib allows you to manipulate, create, modify gettext .pot/.po/.mo
files.  You can load existing files, iterate through it's entries, add,
modify entries, comments or metadata, etc... or create new po/pot files
from scratch.

polib provides a simple and pythonic API, exporting only two
convenience functions 'pofile' and 'mofile', and the 4 core classes:
POFile, MOFile, POEntry and MOEntry for creating new files/entries.

Basic example:
--------------
>>> import polib
>>> # load an existing po file
>>> po = polib.pofile('tests/test.po')
>>> for entry in po:
...     # do something with entry...
...     pass
>>> # add an entry
>>> entry = polib.POEntry()
>>> entry.msgid  = 'Welcome'
>>> entry.msgstr = 'Bienvenue'
>>> entry.occurences  = [('welcome.py', '12'), ('anotherfile.py', '34')]
>>> po.append(entry)
>>> # save our modified po file
>>> po.save()

Some references:
----------------
* Gettext manual: http://www.gnu.org/software/gettext/manual/
* PO file format:
  http://www.gnu.org/software/gettext/manual/html_node/gettext_9.html
* MO file format:
  http://www.gnu.org/software/gettext/manual/html_node/gettext_136.html
"""
### }}}

__author__   = 'David JEAN LOUIS <izimobil@gmail.com>'
__version__  = '0.1.0'


### dependencies {{{
try:
    import struct
    import textwrap
except ImportError, exc:
    raise ImportError('polib requires python 2.3 or later with the ' \
        'standard modules "struct" and "textwrap" (details: %s)' % exc)
### }}}

__all__ = ['pofile', 'POFile', 'POEntry', 'mofile', 'MOFile', 'MOEntry']

### shortcuts for performance improvement and misc variables {{{
# yes, yes, this is quite ugly but *very* efficient
_dictget    = dict.get
_listappend = list.append
_listpop    = list.pop
_strjoin    = str.join
_strsplit   = str.split
_strstrip   = str.strip
_strstartsw = str.startswith
_strendsw   = str.endswith
_strreplace = str.replace
_textwrap   = textwrap.wrap
### }}}


def pofile(fpath, wrapwidth=78):
    """
    Convenience function that parse the po/pot file <fpath> and return
    a POFile instance.

    Keyword argument:
    -----------------
    fpath     -- full or relative path to the po/pot file to parse
    wrapwidth -- integer, the wrap width, only useful when -w option
                 was passed to xgettext, default to 78 (optional)

    Example:
    --------
    >>> import polib
    >>> po = polib.pofile('tests/test.po')
    >>> po #doctest: +ELLIPSIS
    <POFile instance at ...>
    """
    ### pofile {{{
    try:
        fhandle = open(fpath, 'r+')
    except IOError, err:
        raise IOError('Unable to open the po file "%s": %s' % (fpath, err))
    parser = _POFileParser(fhandle)
    instance = parser.parse()
    instance.wrapwidth = wrapwidth
    return instance
    ### }}}


def mofile(fpath, wrapwidth=78):
    """
    Convenience function that parse the mo file <fpath> and return
    a MOFile instance.

    Keyword arguments:
    ------------------
    fpath     -- full or relative path to the mo file to parse
    wrapwidth -- integer, the wrap width, only useful when -w option
                 was passed to xgettext to generate the po file that
                 was used to format the mo file, default to 78
                 (optional).

    Example:
    --------
    >>> import polib
    >>> mo = polib.mofile('tests/test.mo')
    >>> mo #doctest: +ELLIPSIS
    <MOFile instance at ...>
    """
    ### mofile {{{
    try:
        fhandle = open(fpath, 'r+')
    except IOError, err:
        raise IOError('Unable to open the mo file "%s": %s' % (fpath, err))
    parser = _MOFileParser(fhandle)
    instance = parser.parse()
    instance.wrapwidth = wrapwidth
    return instance
    ### }}}


class _BaseFile(list):
    """
    Common parent class for POFile and MOFile classes.
    This class must *not* be instanciated directly.
    """
    ### class _BaseFile {{{

    def __init__(self, fhandle=None, wrapwidth=78):
        """_BaseFile constructor."""
        list.__init__(self)
        # the opened file handle
        self.fhandle = fhandle
        # the width at which lines should be wrapped
        self.wrapwidth = wrapwidth
        # both po and mo files have metadata
        self.metadata = {}
        self.metadata_is_fuzzy = 0

    def __del__(self):
        """
        Destructor: close the file handle when the object is
        destroyed.
        """
        if self.fhandle is not None:
            self.fhandle.close()

    def __str__(self):
        """Common string representation for po and mo files."""
        ret = ''
        if self.metadata_is_fuzzy:
            ret = '#, fuzzy\n'
        ret += 'msgid ""\n'
        ret += 'msgstr ""\n'
        for name, value in self.ordered_metadata():
            values = _strsplit(value, '\n')
            for i, value in enumerate(values): # handle multiline metadata
                if i == 0:
                    ret += '"%s: %s"\n' % (name, _strstrip(value))
                else:
                    ret += '"%s"\n' % _strstrip(value)
        ret += '\n'
        padding = ''
        for entry in self:
            ret += '%s%s' % (padding, entry.__str__(self.wrapwidth))
            padding = '\n'
        return ret.rstrip()

    def __repr__(self):
        """Return the official string representation of the object."""
        return '<%s instance at %d>' % (self.__class__.__name__, id(self))

    def save(self, fpath=None, repr_method='__str__'):
        """
        Save the po file to file <fpath> if no file handle exists for
        the object. If there's already an open file and no fpath is
        provided, then the existing file is rewritten with the modified
        data.
        """
        if self.fhandle is None and fpath is None:
            raise IOError('You must provide a file path to save() method')
        contents = getattr(self, repr_method)()
        if fpath is None:
            self.fhandle.seek(0)
            self.fhandle.truncate()
        else:
            self.fhandle = open(fpath, 'w')
        self.fhandle.write(contents)

    def ordered_metadata(self):
        """
        Convenience method that return the metadata ordered. The return
        value is list of tuples (metadata name, metadata_value).
        """
        # copy the dict first
        metadata = self.metadata.copy()
        data_order = [
            'Project-Id-Version',
            'Report-Msgid-Bugs-To',
            'POT-Creation-Date',
            'PO-Revision-Date',
            'Last-Translator',
            'Language-Team',
            'MIME-Version',
            'Content-Type',
            'Content-Transfer-Encoding'
        ]
        ordered_data = []
        for data in data_order:
            try:
                value = metadata.pop(data)
                _listappend(ordered_data, (data, value))
            except KeyError:
                pass
        # the rest of the metadata won't be order there's no specs for this
        for data in sorted(metadata.keys()):
            value = metadata[data]
            _listappend(ordered_data, (data, value))
        return ordered_data

    def charset(self):
        """
        Return the file encoding charset.
        If the charset cannot be found in metadata, the function
        returns None.
        """
        try:
            charset = _strsplit(self.metadata['Content-Type'], 'charset=')[1]
            charset = _strstrip(charset)
        except LookupError:
            charset = None
        if charset == '':
            charset = None
        return charset

    def to_binary(self):
        """Return the mofile binary representation."""
        raise NotImplementedError('MOFile.to_binary is not yet implemented.')
    ### }}}


class POFile(_BaseFile):
    '''
    Po (or Pot) file reader/writer.
    POFile objects inherit the list objects methods.

    Methods:
    --------
    __str__() -> string
        Return the string representation of the object.

    __repr__() -> string
        Return the "official" string reprsentation of the object.

    to_binary()
        Return the binary (mo format) representation of the file.

    save([fpath])
        Save the string representation to file <fpath>.
        If fpath is ommited and the object points to an existing file,
        the existing file is updated.

    save_as_mofile(fpath)
        Save the binary representation of the file (mo format) to file
        <fpath>.

    percent_translated() -> integer
        Convenience method that return an integer that correspond to
        the percentage of translated messages.

    translated_entries() -> list
        Convenience method that return the list of translated entries.

    untranslated_entries() -> list
        Convenience method that return the list of untranslated
        entries.

    fuzzy_entries() -> list
        Convenience method that return the list of 'fuzzy' entries.

    obsolete_entries() -> list
        Convenience method that return the list of obsolete entries.

    ordered_metadata() -> list
        Convenience method that return the metadata ordered. The return
        value is list of tuples (metadata name, metadata_value).

    charset() -> string
        Return the file encoding charset.
        If the charset cannot be found in metadata, the function
        returns None.

    Properties:
    -----------
    header    -- a string containing the po file header
    metadata  -- a dict containing the po file metadata
    wrapwidth -- an integer, the width at which lines should be wrapped

    An example:
    -----------
    >>> po = POFile()
    >>> entry1 = POEntry("Some english text", "Un texte en anglais")
    >>> entry2 = POEntry("I need my dirty cheese", "Je veux mon sale fromage")
    >>> po.append(entry1)
    >>> po.append(entry2)
    >>> po.header = "Some Header"
    >>> print po
    # Some Header
    msgid ""
    msgstr ""
    <BLANKLINE>
    msgid "Some english text"
    msgstr "Un texte en anglais"
    <BLANKLINE>
    msgid "I need my dirty cheese"
    msgstr "Je veux mon sale fromage"
    '''
    ### class POFile {{{

    def __init__(self, fhandle=None, wrapwidth=78):
        """
        POFile constructor.
        """
        # the po file header
        self.header = ''
        _BaseFile.__init__(self, fhandle, wrapwidth)

    def __str__(self):
        """Return the string representation of the po file"""
        ret, headers = '', _strsplit(self.header, '\n')
        for header in headers:
            if _strstartsw(header, ',') or _strstartsw(header, ':'):
                ret += '#%s\n' % header
            else:
                ret += '# %s\n' % header
        return ret + _BaseFile.__str__(self)

    def save_as_mofile(self, fpath):
        """
        Save the binary representation of the file (mo format) to file
        <fpath>.
        """
        _BaseFile.save(self, fpath, 'to_binary')

    def percent_translated(self):
        """
        Convenience method that return the percentage of translated
        messages.
        """
        total = len(self)
        translated = len(self.translated_entries())
        return int((100.00 / float(total)) * translated)

    def translated_entries(self):
        """
        Convenience method that return a list of translated entries.
        """
        return [e for e in self if e.translated() and not e.obsolete]

    def untranslated_entries(self):
        """
        Convenience method that return a list of untranslated entries.
        """
        return [e for e in self if not e.translated() and not e.obsolete]

    def fuzzy_entries(self):
        """
        Convenience method that return the list of 'fuzzy' entries.
        """
        return [e for e in self if 'fuzzy' in e.flags]

    def obsolete_entries(self):
        """
        Convenience method that return the list of obsolete entries.
        """
        return [e for e in self if e.obsolete]
    ### }}}


class MOFile(_BaseFile):
    '''
    Mo file reader/writer.
    MOFile objects inherit the list objects methods.


    Methods:
    --------
    __str__() -> string
        Return the string representation of the object (as the po
        format).

    __repr__() -> string
        Return the "official" string reprsentation of the object.

    to_binary()
        Return the binary (mo format) representation of the file.

    save([fpath])
        Save the binary representation to file <fpath>.
        If fpath is ommited and the object points to an existing file,
        the existing file is updated.

    save_as_pofile(fpath)
        Save the string representation of the file (po format) to file
        <fpath>.

    ordered_metadata() -> list
        Convenience method that return the metadata ordered. The return
        value is list of tuples (metadata name, metadata_value).

    charset() -> string
        Return the file encoding charset.
        If the charset cannot be found in metadata, the function
        returns None.

    Properties:
    -----------
    magic_number -- the magic number of the binary file
    version      -- the version number of the binary file format
                    (always 0)
    metadata     -- a dict containing the mo file metadata
    wrapwidth    -- an integer, the width at which lines should be
                    wrapped

    An example:
    -----------
    >>> mo = MOFile()
    >>> entry1 = POEntry("Some english text", "Un texte en anglais")
    >>> entry2 = POEntry("I need my dirty cheese", "Je veux mon sale fromage")
    >>> mo.append(entry1)
    >>> mo.append(entry2)
    >>> print mo
    msgid ""
    msgstr ""
    <BLANKLINE>
    msgid "Some english text"
    msgstr "Un texte en anglais"
    <BLANKLINE>
    msgid "I need my dirty cheese"
    msgstr "Je veux mon sale fromage"

    '''
    ### class MOFile {{{

    def __init__(self, fhandle=None, wrapwidth=78):
        """MOFile constructor."""
        _BaseFile.__init__(self, fhandle, wrapwidth)
        self.magic_number = None
        self.version = 0

    def save_as_pofile(self, fpath):
        """
        Save the string representation of the file (po format) to file <fpath>.
        """
        _BaseFile.save(self, fpath)

    def save(self, fpath):
        """
        Save the string representation of the file (po format) to file <fpath>.
        """
        _BaseFile.save(self, fpath, 'to_binary')
    ### }}}


class _BaseEntry:
    """
    Base class for POEntry or MOEntry objects.
    This class must *not* be instanciated directly.
    """
    ### class _BaseEntry {{{
    def __init__(self, msgid='', msgstr=''):
        """Base Entry constructor."""
        self.msgid = msgid
        self.msgstr = msgstr
        self.msgid_plural = ''
        self.msgstr_plural = {}
        self.obsolete = 0
        self.has_plural = 0
        # private vars
        self._is_multiline_msgid = 0
        self._is_multiline_msgid_plural = 0
        self._is_multiline_msgstr = 0
        self._is_multiline_msgstr_plural = {}

    def __repr__(self):
        """Return the official string representation of the object."""
        return '<%s instance at %d>' % (self.__class__.__name__, id(self))

    def __str__(self, wrapwidth=78):
        """
        Common string representation of the POEntry and MOEntry
        objects.
        """
        if self.obsolete:
            delflag = '#~ '
        else:
            delflag = ''
        # write the msgid
        lines = _strsplit(self.msgid, '\n')
        if self._is_multiline_msgid:
            ret = '%smsgid ""\n' % delflag
            for msgid in lines:
                ret += '%s"%s"\n' % (delflag, msgid)
        else:
            ret = '%smsgid "%s"\n' % (delflag, lines[0])
        # write the msgid_plural if any
        if self.msgid_plural:
            lines = _strsplit(self.msgid_plural, '\n')
            if self._is_multiline_msgid_plural:
                ret += '%smsgid_plural ""\n' % delflag
                for msgid in lines:
                    ret += '%s"%s"\n' % (delflag, msgid)
            else:
                ret += '%smsgid_plural "%s"\n' % (delflag, lines[0])
        if self.msgstr_plural:
            msgstrs = self.msgstr_plural
        else:
            msgstrs = {0:self.msgstr}
        for index in sorted(msgstrs.keys()):
            msgstr = msgstrs[index]
            lines = _strsplit(msgstr, '\n')
            plural_index = ''
            is_multiline_msgstr = self._is_multiline_msgstr
            if self.msgstr_plural:
                plural_index = '[%s]' % index
                try:
                    is_multiline_msgstr = \
                            self._is_multiline_msgstr_plural[index]
                except:
                    pass
            if is_multiline_msgstr:
                ret += '%smsgstr%s ""\n' % (delflag, plural_index)
                for mstr in lines:
                    ret += '%s"%s"\n' % (delflag, mstr)
            else:
                ret += '%smsgstr%s "%s"\n' % (delflag, plural_index, lines[0])
        return ret
    ### }}}


class POEntry(_BaseEntry):
    """
    Represents a po file entry.

    Methods:
    --------
    __str__() -> string
        Return the string representation of the object.

    __repr__() -> string
        Return the "official" string reprsentation of the object.

    translated([fpath]) -> boolean
        Return True if the entry has been translated or False otherwise.

    Properties:
    -----------
    msgid         -- a string containing the msgid to translate
    msgid_plural  -- a string containing the plural msgid
    msgstr        -- a string containing the msgid translation.
    msgstr_plural -- a dictionary containing the translations for
                     0,1,...,n occurences (eg. msgstr[0], msgstr[1],
                     etc...)
    occurences    -- a list containing tuples of file name, line number
                     representing occurences of the msgid in the
                     source code.
    comment       -- a string containing a generated comment
                     (eg. #. some text)
    tcomment      -- a string containing a translator comment
                     (eg. # some text)
    flags         -- a list of strings reprensenting the entry flags
                     (eg: fuzzy, c-format...)

    Examples:
    ---------
    >>> entry = POEntry(
    ...     msgid='Welcome', 
    ...     msgstr='Bienvenue',
    ... )
    >>> entry.occurences = [('welcome.py', 12), ('anotherfile.py', 34)]
    >>> print entry
    #: welcome.py:12 anotherfile.py:34
    msgid "Welcome"
    msgstr "Bienvenue"
    <BLANKLINE>

    >>> entry = POEntry()
    >>> entry.occurences = [('src/spam.c', 32), ('src/eggs.c', 45)]
    >>> entry.tcomment = 'A plural translation'
    >>> entry.flags.append('c-format')
    >>> entry.msgid = 'I have spam but no egg !'
    >>> entry.msgid_plural = 'I have spam and %d eggs !'
    >>> entry.msgstr_plural[0] = "J'ai du jambon mais aucun oeuf !"
    >>> entry.msgstr_plural[1] = "J'ai du jambon et %d oeufs !"
    >>> print entry
    # A plural translation
    #: src/spam.c:32 src/eggs.c:45
    #, c-format
    msgid "I have spam but no egg !"
    msgid_plural "I have spam and %d eggs !"
    msgstr[0] "J'ai du jambon mais aucun oeuf !"
    msgstr[1] "J'ai du jambon et %d oeufs !"
    <BLANKLINE>
    """
    ### class POEntry {{{

    def __init__(self, msgid='', msgstr=''):
        """POEntry constructor."""
        _BaseEntry.__init__(self, msgid, msgstr)
        self.occurences = []
        self.comment    = ''
        self.tcomment   = ''
        self.flags      = []

    def __str__(self, wrapwidth=78):
        """
        Return the string representation of the entry.
        """
        ret = ''
        # comment first, if any (with text wrapping as xgettext does)
        if self.comment != '':
            comments = _strsplit(self.comment, '\n')
            for comment in comments:
                if wrapwidth > 0 and len(comment) > wrapwidth-3:
                    lines = _textwrap(comment, wrapwidth,
                                      initial_indent='#. ',
                                      subsequent_indent='#. ',
                                      break_long_words=False)
                    ret += _strjoin('\n', lines) + '\n'
                else:
                    ret += '#. %s\n' % comment
        # translator comment, if any (with text wrapping as xgettext does)
        if self.tcomment != '':
            tcomments = _strsplit(self.tcomment, '\n')
            for tcomment in tcomments:
                if wrapwidth > 0 and len(tcomment) > wrapwidth-2:
                    lines = _textwrap(tcomment, wrapwidth,
                                      initial_indent='# ',
                                      subsequent_indent='# ',
                                      break_long_words=False)
                    ret += _strjoin('\n', lines) + '\n'
                else:
                    ret += '# %s\n' % tcomment
        # occurences (with text wrapping as xgettext does)
        if self.occurences:
            if wrapwidth > 0:
                # XXX textwrap split words that contain hyphen, this is not 
                # what we want for filenames, so the dirty hack is to 
                # temporally replace hyphens with a char that a file cannot 
                # contain, like "*"
                filestr, pad = '', ''
                for filepath, lineno in self.occurences:
                    filestr += '%s%s:%s' % \
                        (pad,  _strreplace(filepath, '-', '*'), lineno)
                    pad = ' '
                lines = _textwrap(filestr, wrapwidth,
                                  initial_indent='#: ',
                                  subsequent_indent='#: ',
                                  break_long_words=False)
                ret += _strjoin('\n', lines) + '\n'
                # end of the replace hack
                ret = _strreplace(ret, '*', '-')
            else:
                try:
                    ret += '#:'
                    for filepath, lineno in self.occurences:
                        ret += ' %s:%s' % (filepath, lineno)
                except IndexError:
                    pass
        # flags
        if self.flags:
            ret += '#, %s\n' % _strjoin(', ', self.flags)
        return ret + _BaseEntry.__str__(self)

    def translated(self):
        """Return True if the entry has been translated or False"""
        return self.msgstr != '' or self.msgstr_plural
    ### }}}


class MOEntry(_BaseEntry):
    """
    Represents a mo file entry.

    Methods:
    --------
    __str__() -> string
        Return the string representation of the object.

    __repr__() -> string
        Return the "official" string reprsentation of the object.

    Properties:
    -----------
    msgid         -- a string containing the msgid to translate
    msgid_plural  -- a string containing the plural msgid
    msgstr        -- a string containing the msgid translation.
    msgstr_plural -- a dictionary containing the translations for
                     0,1,...,n occurences (eg. msgstr[0], msgstr[1],
                     etc...)

    An example:
    -----------
    >>> entry = MOEntry()
    >>> entry.msgid  = 'translate me !'
    >>> entry.msgstr = 'traduisez moi !'
    >>> print entry
    msgid "translate me !"
    msgstr "traduisez moi !"
    <BLANKLINE>

    """
    ### class MOEntry {{{
    def __init__(self, msgid='', msgstr=''):
        """MOEntry constructor."""
        _BaseEntry.__init__(self, msgid, msgstr)

    def __str__(self, wrapwidth=78):
        """
        Return the string representation of the entry.
        """
        return _BaseEntry.__str__(self, wrapwidth)
    ### }}}


class _POFileParser:
    """
    A finite state machine to parse efficiently and correctly po
    file format.
    """
    ### class _POFileParser {{{
    def __init__(self, fhandle):
        """
        Constructor.
        Keyword argument:
        fhandle -- a opened file object.
        """
        self.instance = POFile()
        self.instance.fhandle = fhandle
        self.transitions = {}
        self.current_entry = POEntry()
        self.current_state = 'ST'
        self.current_token = None
        # two memo flags used in handlers
        self.msgstr_index = 0
        self.entry_obsolete = 0
        # Configure the state machine, by adding transitions.
        # Signification of symbols:
        #     * ST: Beginning of the file (start)
        #     * HE: Header
        #     * TC: a translation comment
        #     * GC: a generated comment
        #     * OC: a file/line occurence
        #     * FL: a flags line
        #     * MI: a msgid
        #     * MP: a msgid plural
        #     * MS: a msgstr
        #     * MX: a msgstr plural
        #     * MC: a msgid or msgstr continuation line
        all_ = ['ST', 'HE', 'GC', 'OC', 'FL', 'TC', 'MS', 'MP', 'MX', 'MI']

        self.add('TC', ['ST', 'HE'],                                     'HE')
        self.add('TC', ['GC', 'OC', 'FL', 'TC', 'MS', 'MP', 'MX', 'MI'], 'TC')
        self.add('GC', all_,                                             'GC')
        self.add('OC', all_,                                             'OC')
        self.add('FL', all_,                                             'FL')
        self.add('MI', ['ST', 'HE', 'GC', 'OC', 'FL', 'TC', 'MS', 'MX'], 'MI')
        self.add('MP', ['TC', 'GC', 'MI'],                               'MP')
        self.add('MS', ['MI', 'MP', 'TC'],                               'MS')
        self.add('MX', ['MI', 'MX', 'MP', 'TC'],                         'MX')
        self.add('MC', ['MI', 'MP', 'MS', 'MX'],                         'MC')

    def parse(self):
        """
        Run the state machine, parse the file line by line and call process()
        with the current matched symbol.
        """
        lines, i, lastlen = self.instance.fhandle.readlines(), 0, 0
        for line in lines:
            line = _strstrip(line)
            if line == '':
                i = i+1
                continue
            if _strstartsw(line, '#~ '):
                line = line[3:]
                self.entry_obsolete = 1
            else:
                self.entry_obsolete = 0
            self.current_token = line
            if _strstartsw(line, 'msgid_plural "'):
                # we are on a msgid plural
                self.process('MP', i+1)
            elif _strstartsw(line, 'msgid "'):
                # we are on a msgid
                self.process('MI', i+1)
            elif _strstartsw(line, 'msgstr['):
                # we are on a msgstr plural
                self.process('MX', i+1)
            elif _strstartsw(line, 'msgstr "'):
                # we are on a msgstr
                self.process('MS', i+1)
            elif _strstartsw(line, '#:'):
                # we are on a occurences line
                self.process('OC', i+1)
            elif _strstartsw(line, '"'):
                # we are on a continuation line or some metadata
                self.process('MC', i+1)
            elif _strstartsw(line, '#, '):
                # we are on a flags line
                self.process('FL', i+1)
            elif _strstartsw(line, '# ') or line == '#':
                if line == '#': line = line + ' '
                # we are on a translator comment line
                self.process('TC', i+1)
            elif _strstartsw(line, '#.'):
                # we are on a generated comment line
                self.process('GC', i+1)
            i = i+1

        if lines and self.current_entry:
            # since entries are added when another entry is found, we must add
            # the last entry here (only if there are lines)
            _listappend(self.instance, self.current_entry)
            # before returning the instance, check if there's metadata and if 
            # so extract it in a dict
            firstentry = self.instance[0]
            if firstentry.msgid == '': # metadata found
                # remove the entry
                firstentry = self.instance.pop(0)
                self.instance.metadata_is_fuzzy = firstentry.flags
                multiline_metadata = 0
                for msg in _strsplit(firstentry.msgstr, '\n'):
                    if msg != '':
                        if multiline_metadata:
                            self.instance.metadata[key] += '\n' + msg
                        else:
                            try:
                                key, val = _strsplit(msg, ':', 1)
                                self.instance.metadata[key] = val
                            except:
                                pass
                        multiline_metadata = not _strendsw(msg, '\\n')
        return self.instance

    def add(self, symbol, states, next_state):
        """
        Add a transition to the state machine.
        Keywords arguments:

        symbol     -- string, the matched token (two chars symbol)
        states     -- list, a list of states (two chars symbols)
        next_state -- the next state the fsm will have after the action
        """
        for state in states:
            action = getattr(self, 'handle_%s' % next_state.lower())
            self.transitions[(symbol, state)] = (action, next_state)

    def process(self, symbol, linenum):
        """
        Process the transition corresponding to the current state and the
        symbol provided.

        Keywords arguments:
        symbol  -- string, the matched token (two chars symbol)
        linenum -- integer, the current line number of the parsed file
        """
        try:
            (action, state) = self.transitions[(symbol, self.current_state)]
        except LookupError:
            raise IOError('Invalid token in po file on line %s !' % linenum)
        if action():
            self.current_state = state

    # state handlers

    def handle_he(self):
        """Handle a header comment."""
        token = self.current_token[2:]
        if self.instance.header != '':
            token = '\n' + token
        self.instance.header += token
        return 1

    def handle_tc(self):
        """Handle a translator comment."""
        if self.current_state in ['MC', 'MS', 'MX']:
            _listappend(self.instance, self.current_entry)
            self.current_entry = POEntry()
        token = self.current_token[2:]
        if self.current_entry.tcomment != '':
            token = '\n' + token
        self.current_entry.tcomment += token
        return 1

    def handle_gc(self):
        """Handle a generated comment."""
        if self.current_state in ['MC', 'MS', 'MX']:
            _listappend(self.instance, self.current_entry)
            self.current_entry = POEntry()
        token = self.current_token[3:]
        if self.current_entry.comment != '':
            token = '\n' + token
        self.current_entry.comment += token
        return 1

    def handle_oc(self):
        """Handle a file:num occurence."""
        if self.current_state in ['MC', 'MS', 'MX']:
            _listappend(self.instance, self.current_entry)
            self.current_entry = POEntry()
        try:
            occurences = _strsplit(self.current_token[3:])
            for occurence in occurences:
                if occurence != '':
                    fil, line = _strsplit(occurence, ':')
                    _listappend(self.current_entry.occurences, (fil, line))
        except:
            pass
        return 1

    def handle_fl(self):
        """Handle a flags line."""
        if self.current_state in ['MC', 'MS', 'MX']:
            _listappend(self.instance, self.current_entry)
            self.current_entry = POEntry()
        try:
            self.current_entry.flags += _strsplit(self.current_token[3:], ', ')
        except:
            pass
        return 1

    def handle_mi(self):
        """Handle a msgid."""
        if self.current_state in ['MC', 'MS', 'MX']:
            _listappend(self.instance, self.current_entry)
            self.current_entry = POEntry()
        try:
            self.current_entry.obsolete = self.entry_obsolete
            self.current_entry.msgid = self.current_token[7:-1]
        except:
            pass
        return 1

    def handle_mp(self):
        """Handle a msgid plural."""
        try:
            self.current_entry.has_plural = 1
            self.current_entry.msgid_plural = self.current_token[14:-1]
        except:
            pass
        return 1

    def handle_ms(self):
        """Handle a msgstr."""
        self.current_entry.msgstr = self.current_token[8:-1]
        return 1

    def handle_mx(self):
        """Handle a msgstr plural."""
        try:
            index, value = self.current_token[7], self.current_token[11:-1]
            self.current_entry.msgstr_plural[index] = value
            self.msgstr_index = index
            self.current_entry._is_multiline_msgstr_plural[index] = 0
        except:
            pass
        return 1

    def handle_mc(self):
        """Handle a msgid or msgstr continuation line."""
        try:
            if self.current_state == 'MI':
                pad = self.current_entry.msgid != '' and '\n' or ''
                self.current_entry.msgid += pad + self.current_token[1:-1]
                self.current_entry._is_multiline_msgid = 1
            elif self.current_state == 'MP':
                pad = self.current_entry.msgid_plural != '' and '\n' or ''
                self.current_entry.msgid_plural += pad + \
                        self.current_token[1:-1]
                self.current_entry._is_multiline_msgid_plural = 1
            elif self.current_state == 'MS':
                pad = self.current_entry.msgstr != '' and '\n' or ''
                self.current_entry.msgstr += pad + self.current_token[1:-1]
                self.current_entry._is_multiline_msgstr = 1
            elif self.current_state == 'MX':
                msgstr = self.current_entry.msgstr_plural[self.msgstr_index]
                pad = msgstr != '' and '\n' or ''
                msgstr += pad + self.current_token[1:-1]
                self.current_entry.msgstr_plural[self.msgstr_index] = msgstr
                self.current_entry._is_multiline_msgstr_plural\
                        [self.msgstr_index] = 1
        except:
            pass
        return 0
    # }}}


class _MOFileParser:
    """
    A class to parse binary mo files.
    """
    ### class _MOFileParser {{{
    def __init__(self, fhandle):
        """_MOFileParser constructor."""
        self.instance = MOFile()
        self.instance.fhandle = fhandle
        self.fhandle = fhandle

    def parse_magicnumber(self):
        """
        Parse the magic number and raise an exception if not valid.
        """
        magic_number = self._readbinary(fmt='4s')
        # magic number must be 0xde120495 or 0x950412de
        if magic_number not in ['\xde\x12\x04\x95', '\x95\x04\x12\xde']:
            raise IOError('Invalid mo file, magic number is incorrect !')
        self.instance.magic_number = magic_number

    def parse(self):
        """
        Build the instance with the file handle provided in the
        constructor.
        """
        self.parse_magicnumber()
        # parse the version number
        self.instance.version = self._readbinary('L')
        # parse the number of strings
        numofstrings = self._readbinary('L')
        # original strings hash table offset
        msgids_hash_offset = self._readbinary('L')
        # translation strings hash table offset
        msgstrs_hash_offset = self._readbinary('P')
        # move to msgid hash table and read length and offset of msgids
        self.fhandle.seek(msgids_hash_offset)
        msgids_index = []
        for i in range(numofstrings):
            _listappend(msgids_index, self._readbinary('LL'))
        # move to msgstr hash table and read length and offset of msgstrs
        self.fhandle.seek(msgstrs_hash_offset)
        msgstrs_index = []
        for i in range(numofstrings):
            _listappend(msgstrs_index, self._readbinary('LL'))
        # build entries
        for i in range(numofstrings):
            self.fhandle.seek(msgids_index[i][1])
            msgid = self.fhandle.read(msgids_index[i][0])
            self.fhandle.seek(msgstrs_index[i][1])
            msgstr = self.fhandle.read(msgstrs_index[i][0])
            if i == 0: # metadata
                raw_metadata, metadata = _strsplit(msgstr, '\n'), {}
                for line in raw_metadata:
                    tokens = _strsplit(line, ':', 1)
                    if tokens[0] != '':
                        try:
                            metadata[tokens[0]] = _strstrip(tokens[1])
                        except IndexError:
                            metadata[tokens[0]] = ''
                self.instance.metadata = metadata
                continue
            entry = MOEntry(msgid, msgstr)
            _listappend(self.instance, entry)
        return self.instance

    def _readbinary(self, fmt='c'):
        """
        Private method that unpack n bytes of data using format <fmt>.
        It returns a tuple or a mixed value if the tuple length is 1.
        """
        numbytes = struct.calcsize(fmt)
        tup = struct.unpack(fmt, self.fhandle.read(numbytes))
        if len(tup) == 1:
            return tup[0]
        return tup
    ### }}}


if __name__ == '__main__':
    import doctest
    doctest.testmod()
# vim600: fdm=marker tabstop=4 shiftwidth=4 expandtab ai

SeeBorg [![CircleCI](https://circleci.com/gh/Artoria2e5/seeborg.svg?style=svg)](https://circleci.com/gh/Artoria2e5/seeborg)
=======

This version of hmage's seeborg is equipped with Chinese segmentation methods and a
script normalization pass. It may fail you badly, but don't worry, it comes with
totally no tests so you won't find anything wrong.

This thing depends on [OpenCC](https://github.com/byvoid/OpenCC) to compile. It
currently does some shared linking. The version of OpenCC used need to provide
the "simple" interface; when in doubt, use ArchLinux or go build the latest
source yourself.

Outline of edits: https://gist.github.com/Artoria2e5/d432885ee6235b4b456571cbfec86eba

SeeBorg is a random phrase bot based on Markov Chains.

It will sit on IRC channels you configure, will learn the words and their sequences, and will periodically reply with something that is generated from the talk it learned before.

Don't expect it to make much sense, but sometimes it can give very funny responses.

If you configure yourself as owner, you will gain access for IRC trigger commands (those that begin with '!').

It can learn in any language, not just English -- to handle case conversion properly please set the appropriate locale.

Compilation
----------------
```
git clone https://github.com/hmage/seeborg
cd seeborg
make
```

You'll end up with two executables -- `seeborg-irc` and `seeborg-linein`. The line-in binary is useful for offline chat with the bot and to test the modifications done to the dictionary.

Without any parameters, `seeborg-irc` reads configuration settings from `seeborg-irc.cfg` file, located in current working directory. If it doesn't exist, it will create one and terminate, suggesting you to inspect the config file and modify it to suit your needs.

Configuration
-------------
|Command |Default value | Description |
|--------|--------------|-------------|
| server         |                | IRC server to connect to
| serverport     | 6667           | port of the IRC server to connect to
| nickname       | "SeeBorg"      | IRC nickname
| username       | "SeeBorg"      | IRC username
| realname       | "I am Seeborg" | IRC realname
| quitmessage    | "ByeBye..."    | Message when disconnecting from IRC
| learning       | enabled        | Enable/disable learning capability
| stealth        | enabled        | Enable/disable masking as a regular mIRC user
| replyrate      | 1              | Bot's reply rate, in percent
| replynick      | 33             | Bot's reply rate to his nickname, in percent
| replymagic     | 33             | Bot's reply rate to magic words set in 'magicwords'
| autosaveperiod | 600            | Autosave period, in seconds
| channels       | #test #seeborg | Space separated channel list to autojoin on connect
| magicwords     |                | Space separated list of words that bot replies to, useful if bot has alternative nicknames
| owners         |                | Space separated bot owners' nickname list, the owner's hostname will be locked after each bot restart.


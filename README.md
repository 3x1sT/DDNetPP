[![DDraceNetwork](http://ddnet.tw/ddnet-small.png)](http://ddnet.tw)
[![CircleCI Build Status](https://circleci.com/gh/ChillerDragon/DDNetPP/tree/master.png)](https://circleci.com/gh/ChillerDragon/DDNetPP)
================================

Based on DDNet: Our own flavor of DDRace, a Teeworlds mod. See the [website](http://ddnet.tw) for more information.

Download DDNet++ version 0.0.6 for macOS/linux/windows [here on github](https://github.com/ChillerDragon/DDNetPP/releases/tag/v.0.0.6).

Building
--------

To compile DDNet yourself, you can follow the [instructions for compiling Teeworlds](https://www.teeworlds.com/?page=docs&wiki=compiling_everything).

DDNet requires additional libraries, that are bundled for the most common platforms (Windows, Mac, Linux, all x86 and x86_64). Instead you can install these libraries on your system, remove the `config.lua` and `bam` should use the system-wide libraries by default: `libcurl, libogg, libopus, libopusfile`

If you have the libraries installed, but still want to use the bundled ones instead, you can specify so by running `bam config curl.use_pkgconfig=false opus.use_pkgconfig=false opusfile.use_pkgconfig=false ogg.use_pkgconfig=false`.

The MySQL server is not included in the binary releases and can be built with `bam server_sql_release`. It requires `libmariadbclient-dev` and `libmysqlcppconn-dev`, which are also bundled for the common platforms.

DDNet++
--------

DDnet++ is the upgrade on top of the teeworlds mod ddracenetwork.
Mainly maintained by ChillerDragon.
We added all the features missing in ddnet. For example:
- bloody
- rainbow
- accounts
- xp
- money
- block system (count kills and stats)
- minigames (bomb/fng/blockwave)
- quests
- shop
- police
- jail
- and much more

Known bugs
--------

The map needs the blue and the red flag from CTF gametype or the server crashes.

The survival minigame needs as many spawns as the server has slots.
Use ``/admin test`` command to check if your map has enough spawn.
If it throws an error your server will crash if a survival game starts.
Warnings mean that increasing your slots will crash the server.

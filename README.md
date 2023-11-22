# MapleEzorsia v2 (Standalone HD dll client/localhost for v83)
#### Please follow [setup instructions here](https://github.com/444Ro666/MapleEzorsia-v2/wiki/v83%E2%80%90Client%E2%80%90Setup%E2%80%90and%E2%80%90Development%E2%80%90Guide) for optimal setup, [gameplay after optimal setup](https://www.youtube.com/watch?v=HxGKn0EjPC0). see the [change log to make sure you're up to date](https://github.com/444Ro666/MapleEzorsia-v2/wiki/Change-Log).     Is your game not working? [Go to Troubleshooting Section](https://github.com/444Ro666/MapleEzorsia-v2/wiki/v83%E2%80%90Client%E2%80%90Setup%E2%80%90and%E2%80%90Development%E2%80%90Guide#troubleshooting) of the setup guide and see if any fixes help      _________IMPORTANT NOTE: make sure you re-download config.ini and dependent files when updating, there could be changes

#### 2023/11/17 - in addition to its original functionality, this DLL is now an independent standalone localhost that will work when dropped into any fresh install along with the config

##### NOTE:     so i've notice some ppl dunno what this is. to clarify: this is a cr@ck that turns the default .exe (from NXXXON) into a HD client/localhost useable for singleplayer or with any server.
##### it still works for it's original design of turning any previously released localhost into HD as well, and also has a few other configurable capabilities specified by the user
##### it doesn't need an injector or other extra .exe file to work because MapleStory already loads up dinput8.dll automatically. just needs the dll to be in the same folder. i did this purposely because so many .exe files in the hacking scene are viruses that make you mine coin
##### first to be released, one of a kind, and truly unique in this scene(or any hacking scene): a cr@ck that is open source, with zero strings attached, designed to be the safest, most transparent setup any end user can get

#### 2023/11/17 - if you used this before this date, `ijl15.dll` is no longer supported, instead re-follow the instructions and put `dinput8.dll` in the folder, delete `ijl15.dll`, and rename `2ijl15.dll`(the original) back to `ijl15.dll`

#### 2023/10/23 - if you used this before this date, `nmconew.dll` is no longer supported, instead re-follow the instructions and name `ijl15.dll` to `2ijl15.dll`, place new `ijl15.dll` in the folder, delete old `nmconew.dll`, and rename `nmconew2.dll` back to `nmconew.dll`

This DLL modifies addresses in a default, packed, v83 MapleStory client fresh from installation to enable the game to run; as well as changes the game window and canvas resolution to HD.  
Most in-game component boundaries are also modified to scale with the custom resolution. 
  
The original goal of this DLL is to allow an old 4:3 aspect ratio game to work on 16:9 aspect ratio modern monitors while perserving the original gameplay experience as much as possible. However i also do believe that it is the individual player's choice on how they wish to play the game so i am willing to be flexible regarding optional functionality. As such this project has expanded into also becoming a development base that allows plenty of room for expansion and allows the user to play the game their way.
  
This standalone DLL client is designed to provide a quality, relatively safe, open source client setup to work alongside open source servers. I do not have the skill or resources available to make a fully working custom client from scratch so this is the best i can do
  
This DLL will work with any client or localhost, including the one that comes from a default installation of the game. Deletion of files that come from a default installation is also unnecessary for it to work.
  
This code can only be compiled on the x86 platform. ([Compilation settings](https://www.mediafire.com/view/9ssrqg8eiwbh0k1/buildsettings2.png/file)) (if DLL from compilation settings doesn't work [try this](https://www.mediafire.com/view/6xk7b9bf8qv3gda/buildsettingsEzorsia.png/file))
  
- [HD client setup instructions/development guide](https://github.com/444Ro666/MapleEzorsia-v2/wiki/v83%E2%80%90Client%E2%80%90Setup%E2%80%90and%E2%80%90Development%E2%80%90Guide)
- Original Ezorsia: https://github.com/izarooni/MapleEzorsia
- Current most frequently updated open source v83 server source (checked 2023/10/10): [cosmic](https://github.com/P0nk/Cosmic)
- Coming from another version and need a good localhost? [check out hendi's releases](https://forum.ragezone.com/threads/localhost-workshop.1202021/)

#### Default config.ini
The default `config.ini` file is included. put it in your game folder (where the .dll is) and the program will try to use it. try to avoid extending the comments (behind ;comment) inside of it or INIReader will be unable to read from it and crash you


#### How to use (DLL only)
In your v83 MapleStory directory,  
1. insert [this dinput8.dll from here](https://github.com/444Ro666/MapleEzorsia-v2/releases) in the game directory
2. Run your Game and see how it looks. if you are satisfied then play!
   - if not satisfied or if the game didnt open, close the game, and open the `config.ini` that is generated and edit the values to your liking!
      - for compatibility with your own wz edits see the [troubleshooting section](https://github.com/444Ro666/MapleEzorsia-v2/wiki/v83%E2%80%90Client%E2%80%90Setup%E2%80%90and%E2%80%90Development%E2%80%90Guide#troubleshooting). i suggest doing all your edits in your own EzorsiaV2_UI.wz or MapleEzorsiaV2wzfiles.img (if using .IMG) file to avoid WZ conflicts with the base game, and to ensure you can keep track of them. You can load your edits from these custom files by using the stringpool hook provided in the release source code.
3. Run your Game and enjoy!
4. Game still not working? Try manually downloading [the config and WZ/IMG files here](https://github.com/444Ro666/MapleEzorsia-v2/releases) and inserting them into the appropriate locations manually
   - all files except .IMG go into the same folder as your game, .IMG file is only for ppl who use .IMG files, and goes in the Data folder (which only exists for .IMG users)
- Optional stuff:  [if you want borderless fullscreen and better graphics check out magpie](https://github.com/Blinue/Magpie) i personally use this when playing on 1280x720 to force it to cover my screen without graphic quality loss. [default v83 client with admin elevation request removed, and 4G flag enabled](https://mega.nz/file/9uNmHIAZ#zzE7t7T6wQyDbJrHJxgw-AOlmzzwCpLrOKmoUlec_5E): this is the default client but doesn't request admin, and can use 4g memory instead of 2g [see my setup instructions here if you want to do the edits yourself for verification](https://github.com/444Ro666/MapleEzorsia-v2/wiki/v83%E2%80%90Client%E2%80%90Setup%E2%80%90and%E2%80%90Development%E2%80%90Guide)

## Features
- the first publically released standalone dll client for v83 (functionality added on 11/16/2023)
- no WZ/IMG conflicts; Ezorsia V2 will generate its only WZ/IMG file, compatible with any set of WZ or IMG files, provided it is configured correctly!
- the ability to load 3 custom third party dll that you can specify in the config, these must be in the game folder; Ezorsia v2's edits will take precedence over conflicting edits (sometimes a good thing if those dll files have things that prevent the game running, and Ezorsia v2 overwrites those same things)
- pick up and exp gain messages that were centered on the character in the original ezorsia is fixed and moved to the bottom right  
	- the messages also no longer appear cut off at the beginning  
	- the functionality of announcing the messages has been improved, and the player can now configure how many pick up and exp gain messages they want to display	instead of being stuck with 6 maximum (the recommended maximum amount is half your resolution height divided by 14, this will make the messages go up to half your screen)  
- cash shop has been fully fixed, and it now displays as 800x600, centered in the middle of the screen regardless of resoluton (you still need UI.wz edit if you don't want the medal text under character to show outside the left of the preview screen)  
	- cash shop preview bug from vanilla v83 has been fixed and now you can move your character right away in preview upon entering the cash shop. normally you'd have to toggle previewing off and then on again for it to work (vanilla bug)  
- login, world/channel, character select, screens have been fixed and is now centered, and it now displays as 800x600, centered in the middle of the screen regardless of resolution (you still need UI.wz edit for the best look, but it works with vanilla login frame as well if EzorsiaV2WzIncluded=false and CustomLoginFrame=false in config)  
	- version number, view recommended, buttons, and other UI element related to these screens are centered  
- fully scaled mulung dojo UI, all elements in right spots for each resolution  
- fully scaled "clear" pop up for small bosses (tested on Capt latanica)  
- systems and shortcuts blue pop-up menus have their locations fixed  
- various pop-up messages, invites, and requests like party, guild, trade, family, quest complete, party quest available, have been fixed  
- megaphone avatar location fixed  
- normal boss bars scale with resolution  
- window mode that can be turned on and off  
- logos on game load that can be turned on and off  
- all of the resolution edits that came with original ezorsia (ones that cause issues has been commented out but still kept for reference)  
- most of the resolution edits from the [ragezone resolution addresses release](https://forum.ragezone.com/threads/all-addresses-for-v83-resolution-change.1161938/), with problematic ones removed and addresses not repeated versus the original ezorsia addresses
- these are only the features/fixes at release, [there have been additions](https://github.com/444Ro666/MapleEzorsia-v2/wiki/Change-Log) after release
   


#### improvements in source from original:
- too many changes to count, see [change log](https://github.com/444Ro666/MapleEzorsia-v2/wiki/Change-Log) and source xD
- contains examples of function replacement and codecaves  
- occasional helpful comments for further development  
- numerous addresses, some of which aren't used because they aren't related to resolution  
- key edits/addresses have functionality identified (these cause noticeable issues when messed with)

    (note: i have not fully identified all address functionality but i have playtested the game and commented out or modified ones that caused issues; i've also reorganized a lot of the addresses to be closer other addresses in the same memory region)

### Want to help me?
My [To-do](https://github.com/444Ro666/MapleEzorsia-v2/wiki/my-to%E2%80%90do-list) list is what im currently looking to improve on for the patch in the future. development will be slower now that it is releasable compared to pre-release.
[click here](https://github.com/444Ro666/MapleEzorsia-v2/blob/main/CONTRIBUTING.md) to see how you can help. you can also see the [guide here](https://github.com/444Ro666/MapleEzorsia-v2/wiki/v83%E2%80%90Client%E2%80%90Setup%E2%80%90and%E2%80%90Development%E2%80%90Guide) on how to develop the patch


## Credits
i'd like to thank the members of the [maple dev community](https://discord.gg/DU8j6xrW) who took the time to help me when they did. I'd also like to thank the creator of the [original ezorsia](https://github.com/izarooni/MapleEzorsia) for creating the base i used to work off of and learn from. A special mention to the creators of [MapleClientEditTemplate](https://github.com/MapleStory-Archive/MapleClientEditTemplate) whose re-written utility functions and type definitions helped transition this project towards more advanced client modifications. Another special mention to [hendi's localhost workshop](https://forum.ragezone.com/threads/localhost-workshop.1202021/) for the guidance about which functions to re-write in order to implement a working standalone client dll, and for providing a unvirtualized localhost to analyze. Finally, I'd like to thank everyone who provided the releases and resources in the [setup and development](https://github.com/444Ro666/MapleEzorsia-v2/wiki/v83%E2%80%90Client%E2%80%90Setup%E2%80%90and%E2%80%90Development%E2%80%90Guide) page; i would not have been able to do this work without the work that which was done before me.

## License

[ AGPL-3.0 license ](https://github.com/444Ro666/MapleEzorsia-v2/blob/main/LICENSE)  Ezorsia v2 uses the same copyleft license as cosmic and it's predecessor heavenms. It is a copyleft license where if the source is used, in part or in whole, in another project, then the resulting "modified" version must also come under the same license (GNU Affero General Public License v3.0)

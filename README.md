# MapleEzorsia v2 (HD client patch for v83)
#### Please follow setup instructions [here](https://github.com/444Ro666/MapleEzorsia-v2/wiki/v83%E2%80%90Client%E2%80%90Setup%E2%80%90and%E2%80%90Development%E2%80%90Guide) for optimal setup
This DLL modifies addresses in a v83 MapleStory client to change the game window and canvas resolution.  
Most in-game component boundaries are also modified to scale with the custom resolution. 

This DLL requires a clean game client like the one provided in setup, using a client with unknown binary edits can result in unstable and unpredictable behavior!  

This DLL also requires some minor modifications to UI.wz to work optimally because i'm currently not skilled enough to change the game in those areas using only DLL, these are also provided in the setup.  
(The patch can be used without those UI.wz modifications but you need to turn CustomLoginFrame=false in the `config.ini` file if you aren't using your own custom login frame)

This code can only be compiled on the x86 platform. (Compilation settings: https://www.mediafire.com/view/6xk7b9bf8qv3gda/buildsettingsEzorsia.png/file)

- HD client setup instructions/development guide: [here](https://github.com/444Ro666/MapleEzorsia-v2/wiki/v83%E2%80%90Client%E2%80%90Setup%E2%80%90and%E2%80%90Development%E2%80%90Guide)
- Original Ezorsia: https://github.com/izarooni/MapleEzorsia
- Current most frequently updated open source server source (checked 2023/10/10): [cosmic](https://github.com/P0nk/Cosmic)

#### Default config.ini
The default `config.ini` file is included. put it in your game folder (where the .dll is) and the program will try to use it. try to avoid extending the comments (behind ;comment) inside of it or INIReader will be unable to read from it and cause the patch to default to default values for all of the toggle-able values


#### How to use (DLL only)
In your v83 MapleStory directory,  
1. Rename file `nmconew.dll` to `nmconew2.dll`  
2. insert [this DLL file](https://github.com/444Ro666/MapleEzorsia-v2/releases) in that directory
3. insert config.ini in that same directory and check to make the values are as they should be; CustomLoginFrame=false if you aren't using the UI.wz from the [setup guide](https://github.com/444Ro666/v83-Client-Setup-and-Development-Guide) or your own custom login frame
4. Run your client!

## Features
- pick up and exp gain messages that were centered on the character in the original ezorsia is fixed and moved to the bottom right  
	- the messages also no longer appear cut off at the beginning  
	- the functionality of announcing the messages has been improved, and the player can now configure how many pick up and exp gain messages they want to display	instead of being stuck with 6 maximum (the recommended maximum amount is half your resolution height divided by 14, this will make the messages go up to half your screen)  
- cash shop has been fully fixed, and it now displays as 800x600, centered in the middle of the screen regardless of resoluton (you still need UI.wz edit if you don't want the medal text under character to show outside the left of the preview screen)  
	- cash shop preview bug from vanilla v83 has been fixed and now you can move your character right away in preview upon entering the cash shop. normally you'd have to toggle previewing off and then on again for it to work (vanilla bug)  
- login, world/channel, character select, screens have been fixed and is now centered, and it now displays as 800x600, centered in the middle of the screen regardless of resolution (you still need UI.wz edit for the best look, but it works with vanilla login frame as well if CustomLoginFrame=false in config)  
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
   


#### improvements in source from original:
- contains examples of function replacement and codecaves  
- occasional helpful comments for further development  
- numerous addresses, some of which aren't used because they aren't related to resolution  
- key edits/addresses have functionality identified (these cause noticeable issues when messed with)


    (note: i have not fully identified all address functionality but i have playtested the game and commented out or modified ones that caused issues; i've also reorganized a lot of the addresses to be closer other addresses in the same memory region)

### Want to help me?
My [To-do](https://github.com/444Ro666/MapleEzorsia-v2/wiki/my-to%E2%80%90do-list) list is what im currently looking to improve on for the patch in the future. development will be slower now that it is releasable compared to pre-release.
[click here](https://github.com/444Ro666/MapleEzorsia-v2/blob/main/CONTRIBUTING.md) to see how you can help


## Credits
i'd like to thank the members of the [maple dev community](https://discord.gg/DU8j6xrW) who took the time to help me when they did. I'd also like to thank the creator of the [original ezorsia](https://github.com/izarooni/MapleEzorsia) for creating the base i used to work off of and learn from. I'd also like to thank the people who provided the releases and resources in the [setup and development](https://github.com/444Ro666/v83-Client-Setup-and-Development-Guid) page; i would not have been able to do this work without the work that which was done before me.

## License

[ AGPL-3.0 license ](https://github.com/444Ro666/MapleEzorsia-v2/blob/main/LICENSE)  Ezorsia v2 uses the same copyleft license as cosmic and it's predecessor heavenms. It is a copyleft license where if the source is used, in part or in whole, in another project, then the resulting "modified" version must also come under the same license (GNU Affero General Public License v3.0)

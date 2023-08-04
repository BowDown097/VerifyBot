# Epic Games Verification Guide
1. Go to https://legendary.gl/epiclogin and accept the authorization message if there is one. You should see a page (JSON file) that shows a ``redirectUrl``, ``authorizationCode``, and ``sid``.
2. Run the **/verifyepic** command in the #verify channel and paste the ``authorizationCode`` from the above step inside of the dialog. If you don't know what you're doing, see the below images.

### Firefox
<img src="https://github.com/BowDown097/VerifyBot/assets/42720004/f0cd87ec-4e2f-488b-9523-cab9bf63f404" width="416" height="310" />

### Chromium (Chrome, Edge, Brave, etc)
<img src="https://github.com/BowDown097/VerifyBot/assets/42720004/a81df0df-696a-4ceb-bbb5-ce1c4fa2b03f" width="562" height="147" />

## How can I trust this to not do anything spooky?
Well, thing is you really can't 100%, but I've done the most I can to make it as safe as possible. Here's the facts:
- No information is exposed to other users of the server.
- The bot is completely open source. You can check the code for anything spooky.
- Your authorization code and access token are invalidated right after the command is ran.
- The server's really big and I'd say I like it. I don't care about Epic Games stuff and it would be stupid to throw everything away for it.

If there's anything I can do to make it less spooky on the code side of things, then create an issue with some details, but I don't think more can be done.

Description
-----------
Gives a site owner options to disable specific messages shown to end users. The core drupal message system as offered by drupal_set_message is an excellent way for modules to send out messages to the end users. However not all drupal site owners are keen to show all the messages sent out by drupal core and all modules to their users. This module gives site administrators a reasonably powerful way to filter out messages shown to the end users.

Features
--------
1. Filter out messages that match a full text string exactly.
2. Filter out messages that match a regular expression.
3. Permissions to specifically hide all messages of a given type from any role.
4. Disable all filtering for specific users.
5. Disable all filtering for specific paths.
6. Apply filtering only for specific paths.
7. Debug system to get messages in the HTML without showing it to the end users.

Configuration
-------------
1. Visit 'administer >> site configuration >> disable messages'
2. Add the specific messages you wish to filter out to the 'Filter messages' text area.
3. Visit 'administer >> user management >> permissions' 
4. Assign the 'view <type> message' to roles who should be able to see the given <type> of messages. Users who do not have the permissions to see a given type of messages will not be able to see any of the messages of the given type. Useful to hide warning and error messages from end users on a production site.
5. If so desired you can also specify the pages where filtering should be applied or excluded by opening the 'Page and user level filtering options' fieldset and setting the 'Apply filters by page' radio and textarea.
6. You can exclude all kinds of filtering for specific users. (eg: user 1)


Installation
------------
1. Extract the tar.gz into your 'modules' or directory.
2. Enable the module at 'administer >> site building >> modules'.
3. Configure options in admin/settings/disable-messages

Uninstallation
--------------
1. Disable the module.
2. Uninstall the module

Credits
-------
Written by Zyxware, http://www.zyxware.com/

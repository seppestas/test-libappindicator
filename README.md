Libappindicator workaround test
===============================

This repository contains PoCs to work around the limitiations set by
[libappindicator](https://launchpad.net/libappindicator).

The goal is to know when the icon of the application indicator gets clicked
so we can expose it in the [Electron tray API](https://github.com/electron/electron/blob/master/docs/api/tray.md).

For more informations see [bug 522152 of Ubuntu libappindicator](https://bugs.launchpad.net/ubuntu/+source/libappindicator/+bug/522152).

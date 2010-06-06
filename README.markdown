About
-----------
Incallert software makes in-call "minute beeps" for S60 2nd and 3rd Ed phones.

The original version was developed and deployed at <http://www.clearevo.com/incallert>
(As this project is free-software so the source-code is freely available to other developers, so you might be using improved/altered versions of this software.)

Installing
-----------
Install the .sis file for S60 2nd, .sisx for S60 3rd. 
NOTE: S60 3rd phones must go to "Tools" > "App.mgr" > Options > Settings > Software Installation > set to "All"


Compiling
-----------
This project is a general Symbian C++ project so you can probably compile it as any other Symbian C++ project, you might need to change the pkg file for the direct path to each component.

This project is developed/built using gnupoc and eclipse ide in Ubuntu GNU/Linux - for full details on how to set up the compiling/development environment, please visit:
<http://www.clearevo.com/blog/programming/2009/06/05/symbian_c%2B%2B_on_linux_using_gnupoc_and_eclipse_cdt.html>

You will also need to make your own sis folder and your own version of the pkg file: for how and why please refer to <http://wiki.github.com/ykasidit/Incallert/compiling>


License
-----------
Copyright (C) 2009 Kasidit Yusuf.

This file is part of Incallert.

Incallert is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Incallert is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Incallert.  If not, see <http://www.gnu.org/licenses/>.

Developers tools
================


Astyle
------

https://astyle.sourceforge.net/astyle.html

Usage (from source root):

    astyle  --project=tools/astyle.cfg -I path/to/file.cpp

or

    astyle  --project=tools/astyle.cfg -I -r 'module/*.cpp' 'module/*.h'


codespell
---------

https://github.com/codespell-project/codespell

Usage (from source root):

    codespell --config tools/codespell.cfg


Include-What-You-Use
--------------------

https://include-what-you-use.org
    
To use IWYU, run qmake with the optional USE flag:

    qmake  -r USE+=check_iwyu path/to/gcodeworkshop_top.pro

The following additional parameters are possible:

* `IWYU_INCLUDES` specifies additional paths to search for header files,
  this comes in handy in Debian:

      IWYU_INCLUDES=/usr/lib/llvm-13/include/c++/v1

* `IWYU_EXTRA_FLAGS` additional options specifically IWYU:

      IWYU_EXTRA_FLAGS='-Xiwyu --update_comments'


**Ignored proposals**

IWYU v0.21

* Add `#include <memory>` for `__shared_ptr_access` (Qt5.15 & Qt6.4)
  - gcodeworkshop/src/document.cpp
  - gcodeworkshop/src/documentmanager.cpp
  - gcodeworkshop/src/documentproducer.cpp
  - gcodeworkshop/src/gcoderdocument.cpp
  - gcodeworkshop/src/gcodeworkshop.cpp
* Add `#include <algorithm>` for `std::max` (Qt5.15)
  - gcodeshared/src/utils/gcode-converter.cpp
* Remove `#include <QObject>`
  - gcodeworkshop/src/highlighter.h (Qt6.4)
* Replace `#include <QSharedPointer>` with `template <class T> class QSharedPointer`
  - gcodeshared/include/utils/configdialog.h (Qt6.4)
* Replace `class QMdiArea` with `#include <QMdiArea>`
  - gcodeworkshop/include/documentmanager.h (Qt6.4)


**Additional links:**

* https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/IWYUMappings.md
* https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/IWYUPragmas.md


Tips
----

Convert png to ico

    convert application-x-g-code.png  -define icon:auto-resize=256,128,64,48,32,24,16 gcode_file.ico

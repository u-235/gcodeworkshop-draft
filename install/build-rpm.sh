#!/usr/bin/env sh

set -e

#MODE_NO_CLEAN=1

PACKAGE_ARCH=$(uname -m)
PACKAGE_INSTALL_PREFIX=/usr

SCRIPT_DIR=$(realpath "$(dirname "$0")")
SOURCE_DIR=$(realpath "$SCRIPT_DIR"/../)

clean()
{
    if [ -f "${PACKAGE_FILE_NAME}.rpm" ]; then
        echo "Remove the old package ${PACKAGE_FILE_NAME}.rpm"
        rm "${PACKAGE_FILE_NAME}.rpm"
    fi

    if [ -d  "$BUILD_DIR/rpmbuild" ]; then
        echo "Remove $BUILD_DIR/rpmbuild"
        rm -r "$BUILD_DIR/rpmbuild"
    fi
}

pre_build()
{
    QMAKE_OPTIONS="$QMAKE_OPTIONS PREFIX=$PACKAGE_INSTALL_PREFIX"
    echo "$QMAKE -r $QMAKE_OPTIONS ${SOURCE_DIR}"
    $QMAKE -r $QMAKE_OPTIONS ${SOURCE_DIR}
}

build() {
    make -j $MAKE_JOBS
}

i18n()
{
    make -j $MAKE_JOBS i18n
}

deploy()
{
    make INSTALL_ROOT="$PACKAGE_DIR" install
}

package()
{
	# https://docs.fedoraproject.org/en-US/packaging-guidelines/RPMMacros
    cat <<EOF > "$BUILD_DIR/$PACKAGE_NAME.spec"
Summary:   G-code editor
Name:      gcodeworkshop
Version:   ${PACKAGE_VERSION}
Release:   0
License:   GPL-3.0-or-later
Group:     Applications/Develop
Packager:  nicegorov@yandex.ru
URL:       https://github.com/GCodeProjects/GCodeWorkShop

%description
GCodeWorkShop is a text editor for CNC programmers.

%files
${PACKAGE_INSTALL_PREFIX}/bin/gcodefileserver
${PACKAGE_INSTALL_PREFIX}/bin/gcodeworkshop
${PACKAGE_INSTALL_PREFIX}/share/applications/gcodeworkshop.desktop
${PACKAGE_INSTALL_PREFIX}/share/doc/gcodeworkshop/COPYING
${PACKAGE_INSTALL_PREFIX}/share/doc/gcodeworkshop/COPYING.LESSER
${PACKAGE_INSTALL_PREFIX}/share/doc/gcodeworkshop/README.md
${PACKAGE_INSTALL_PREFIX}/share/doc/gcodeworkshop/SerialTransmission_Help.html
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/0100.nc
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/FANUC0M.nc
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/FANUC0T.nc
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/FLANGE.nc
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/HAAS.nc
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/HK.nc
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/OSP5020M.nc
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/OSP7000L.nc
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/PHILIPS.nc
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/SINUMERIK840D.nc
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/SINUMERIK850.nc
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/TOKARKA.NC
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/cnc_tips.txt
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/examples/macro7.nc
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/gcodeworkshop_ca.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/gcodeworkshop_cs_CZ.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/gcodeworkshop_de.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/gcodeworkshop_es.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/gcodeworkshop_fi.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/gcodeworkshop_nl.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/gcodeworkshop_pl.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/gcodeworkshop_ru.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/gcodeworkshop_zh_CN.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/kdiff3_ca.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/kdiff3_cs_CZ.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/kdiff3_de.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/kdiff3_es.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/kdiff3_fi.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/kdiff3_nl.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/kdiff3_pl.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/kdiff3_ru.qm
${PACKAGE_INSTALL_PREFIX}/share/gcodeworkshop/lang/kdiff3_zh_CN.qm
${PACKAGE_INSTALL_PREFIX}/share/icons/hicolor/32x32/mimetypes/application-x-g-code.png
${PACKAGE_INSTALL_PREFIX}/share/icons/hicolor/48x48/apps/edytornc.png
${PACKAGE_INSTALL_PREFIX}/share/mime/packages/application-x-g-code.xml

%changelog

EOF

    title2 "rpmbuild"
    echo "rpmbuild --bb --buildroot "$PACKAGE_DIR" --define "_topdir $BUILD_DIR/rpmbuild" "$BUILD_DIR/$PACKAGE_NAME.spec""
    rpmbuild --bb --buildroot "$PACKAGE_DIR" --define "_topdir $BUILD_DIR/rpmbuild" "$BUILD_DIR/$PACKAGE_NAME.spec"
    mv "$BUILD_DIR/rpmbuild/RPMS/$PACKAGE_ARCH/"*.rpm $PWD/${PACKAGE_FILE_NAME}.rpm
}

__NESTED__=1
. ${SCRIPT_DIR}/build-helper.sh

create package

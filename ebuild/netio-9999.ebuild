EAPI=2

DESCRIPTION="tool for redirect stdin, stdout or program to network connection"
HOMEPAGE="http://www.yurial.ru/netio"

which git &>/dev/null
if [ $? == 0 ]; then
    inherit git

    src_unpack() {
        git_src_unpack
    }

    EGIT_REPO_URI="git://yurial.ru/netio.git"
    if [ "$PV" == "9999" ]; then
        KEYWORDS="~alpha ~amd64 ~arm ~hppa ~ia64 ~mips ~ppc ~s390 ~sh ~sparc ~x86 ~sparc-fbsd ~x86-fbsd"
        EGIT_BRANCH="master"
    else
        KEYWORDS="x86"
        EGIT_BRANCH="$P"
    fi
else
    SRC_URI="http://yurial.ru/netio/${P}.tar.bz2"
    KEYWORDS="x86"
fi

LICENSE="GPL-3"
SLOT="0"
IUSE="debug unixsock minimal +ipv4 ipv6 +tcp udp sctp"
DEPEND=""
S=${WORKDIR}/netio

src_compile() {
	econf $(use_enable debug) $(use_enable minimal) $(use_enable unixsock) $(use_enable ipv4) $(use_enable ipv6) $(use_enable tcp) $(use_enable udp) $(use_enable sctp)
	make
}

src_install() {
    INSTDIR=${BUILD_PREFIX}/${CATEGORY}/${P}/image
    INSTBINDIR=${INSTDIR}/usr/bin
    mkdir -p ${INSTBINDIR}
    cp netio ${INSTBINDIR}
    INSTMANDIR=${INSTDIR}/usr/share/man
    mkdir -p ${INSTMANDIR}/man1
    cp man/man.1.en ${INSTMANDIR}/man1/netio.1
}

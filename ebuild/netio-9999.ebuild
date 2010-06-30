EAPI=2

inherit git

DESCRIPTION="tool for redirect stdin, stdout or program to network connection"
HOMEPAGE="http://www.yurial.ru/netio"

SRC_URI=""
EGIT_BRANCH="master"
EGIT_REPO_URI="git://yurial.ru/netio.git"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="~alpha ~amd64 ~arm ~hppa ~ia64 ~mips ~ppc ~s390 ~sh ~sparc ~x86 ~sparc-fbsd ~x86-fbsd"
IUSE="debug +ipv4 ipv6 +tcp udp sctp"
DEPEND=""
S=${WORKDIR}/netio

src_unpack() {
    git_src_unpack
}

src_compile() {
	econf $(use_with debug) $(use_enable ipv4) $(use_enable ipv6) $(use_enable tcp) $(use_enable udp) $(use_enable sctp)
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

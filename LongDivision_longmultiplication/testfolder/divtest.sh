set srcdir=~csci570b/public/cs570/hw1

set v0=215466667
set v1=833724077
set v2=2119913542
set v3=31171330
set v4=670899175
set v5=568789557
set v6=746837897
set v7=1117999327
set v8=1780376106
set v9=895339095

set v10=1694127260
set v11=298698290
set v12=996256349
set v13=1660676014
set v14=1584872790
set v15=508205057
set v16=533843142
set v17=1987053021
set v18=155998947
set v19=1479914115

set v20=1001
set v21=5
set v22=65535
set v23=97531
set v24=256

set v99=4294967295

#
# for the following commands, each correct answer gets 1 point
#
/bin/rm -f f?.div f??.div

./hw1 div $v0 $v10 > f0.div
./hw1 div $v1 $v11 > f1.div
./hw1 div $v2 $v12 > f2.div
./hw1 div $v3 $v13 > f3.div
./hw1 div $v4 $v14 > f4.div
./hw1 div $v5 $v15 > f5.div
./hw1 div $v6 $v16 > f6.div
./hw1 div $v7 $v17 > f7.div
./hw1 div $v8 $v18 > f8.div
./hw1 div $v9 $v19 > f9.div

foreach f (0 1 2 3 4 5 6 7 8 9)
    echo "===> $srcdir/f$f"
    diff $srcdir/f$f.div f$f.div
end

./hw1 div $v0 $v19 > f10.div
./hw1 div $v1 $v18 > f11.div
./hw1 div $v2 $v17 > f12.div
./hw1 div $v3 $v16 > f13.div
./hw1 div $v4 $v15 > f14.div
./hw1 div $v5 $v14 > f15.div
./hw1 div $v6 $v13 > f16.div
./hw1 div $v7 $v12 > f17.div
./hw1 div $v8 $v11 > f18.div
./hw1 div $v9 $v10 > f19.div

foreach f (10 11 12 13 14 15 16 17 18 19)
    echo "===> $srcdir/f$f"
    diff $srcdir/f$f.div f$f.div
end

./hw1 div $v99 $v20 > f20.div
./hw1 div $v99 $v21 > f21.div
./hw1 div $v99 $v22 > f22.div
./hw1 div $v99 $v23 > f23.div
./hw1 div $v99 $v24 > f24.div

foreach f (20 21 22 23 24)
    echo "===> $srcdir/f$f"
    diff $srcdir/f$f.div f$f.div
end

/bin/rm -f f?.div f??.div

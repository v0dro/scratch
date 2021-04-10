rank = 8

for mc in range(0, rank, 4):
    for nc in range(0, rank, 4):
        print("Cmn -> mc: ", mc, " nc: ", nc)
        for xc in range(0, rank, 4):
            print("\tAx  -> xc: ", xc, " mc: ", mc, "Cmn ->  mc: ", mc , " nc: ", nc)
            for yc in range(0, rank, 4):
                print( "\t\tExn -> xc: ", xc, " nc: ", nc, "Bx -> ", " nc: ", nc ," yc: ", yc)


for mc in range(0, rank, 4):
    for nc in range(0, rank, 4):
        print("Cmn -> mc: ", mc, " nc: ", nc)

for mc in range(0, rank, 4):
    for xc in range(0, rank, 4):
        for nc in range(0, rank, 4):
            print("\tAx  -> xc: ", xc, " mc: ", mc, "Cmn ->  mc: ", mc , " nc: ", nc)

for nc in range(0, rank, 4):
    for xc in range(0, rank, 4):
        for yc in range(0, rank, 4):
            print( "\t\tExn -> xc: ", xc, " nc: ", nc, "Bx -> ", " nc: ", nc ," yc: ", yc)


# Cmn -> mc:  0  nc:  0
# 	 Ax  -> xc:  0  mc:  0  nc:  0
# 	 Exn -> xc:  0  nc:  0  yc:  0
# 	 Exn -> xc:  0  nc:  0  yc:  4


# 	 Ax  -> xc:  4  mc:  0  nc:  0
# 	 Exn -> xc:  4  nc:  0  yc:  0
# 	 Exn -> xc:  4  nc:  0  yc:  4


# Cmn -> mc:  0  nc:  4
# 	 Ax  -> xc:  0  mc:  0  nc:  4
# 	 Exn -> xc:  0  nc:  4  yc:  0
# 	 Exn -> xc:  0  nc:  4  yc:  4


# 	 Ax  -> xc:  4  mc:  0  nc:  4
# 	 Exn -> xc:  4  nc:  4  yc:  0
# 	 Exn -> xc:  4  nc:  4  yc:  4


# Cmn -> mc:  4  nc:  0
# 	 Ax  -> xc:  0  mc:  4  nc:  0
# 	 Exn -> xc:  0  nc:  0  yc:  0
# 	 Exn -> xc:  0  nc:  0  yc:  4


# 	 Ax  -> xc:  4  mc:  4  nc:  0
# 	 Exn -> xc:  4  nc:  0  yc:  0
# 	 Exn -> xc:  4  nc:  0  yc:  4


# Cmn -> mc:  4  nc:  4
# 	 Ax  -> xc:  0  mc:  4  nc:  4
# 	 Exn -> xc:  0  nc:  4  yc:  0
# 	 Exn -> xc:  0  nc:  4  yc:  4


# 	 Ax  -> xc:  4  mc:  4  nc:  4
# 	 Exn -> xc:  4  nc:  4  yc:  0
# 	 Exn -> xc:  4  nc:  4  yc:  4


################################################################

# Cmn -> mc:  0  nc:  0
# 	 Ax  -> xc:  0  mc:  0  nc:  0
# 	 Ax  -> xc:  4  mc:  0  nc:  0
# 	 Exn -> xc:  4  nc:  0  yc:  0
# 	 Exn -> xc:  4  nc:  0  yc:  4


# Cmn -> mc:  0  nc:  4
# 	 Ax  -> xc:  0  mc:  0  nc:  4
# 	 Ax  -> xc:  4  mc:  0  nc:  4
# 	 Exn -> xc:  4  nc:  4  yc:  0
# 	 Exn -> xc:  4  nc:  4  yc:  4


# Cmn -> mc:  4  nc:  0
# 	 Ax  -> xc:  0  mc:  4  nc:  0
# 	 Ax  -> xc:  4  mc:  4  nc:  0
# 	 Exn -> xc:  4  nc:  0  yc:  0
# 	 Exn -> xc:  4  nc:  0  yc:  4


# Cmn -> mc:  4  nc:  4
# 	 Ax  -> xc:  0  mc:  4  nc:  4
# 	 Ax  -> xc:  4  mc:  4  nc:  4
# 	 Exn -> xc:  4  nc:  4  yc:  0
# 	 Exn -> xc:  4  nc:  4  yc:  4

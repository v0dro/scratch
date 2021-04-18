import numpy as np
rank = 8

np.set_printoptions(linewidth=400)

for mc in range(0, rank, 4):
    for nc in range(0, rank, 4):
        print("Cmn -> mc: ", mc, " nc: ", nc)
        for xc in range(0, rank, 4):
            print("\tAx  -> xc: ", xc, " mc: ", mc, "Cmn ->  mc: ", mc , " nc: ", nc)
            for yc in range(0, rank, 4):
                print( "\t\tExn -> xc: ", xc, " nc: ", nc, "Bx -> ", " nc: ", nc ," yc: ", yc)


for mc in range(0, rank, 4):
    for nc in range(0, rank, 4):
        print("Cmn( ", mc, ",", nc, ") = Av( ", mc, ",", ": ) * Bu(", " :, ", nc, " )")

        # print("Cmn -> mc: ", mc, " nc: ", nc)

for mc in range(0, rank, 4):
    for xc in range(0, rank, 4):
        for nc in range(0, rank, 4):
            # print("\tAx  -> xc: ", xc, " mc: ", mc, "Cmn ->  mc: ", mc , " nc: ", nc)
            print("\tExn(", xc, ",", nc , ") = ", " Ax(", xc, "," , mc, ") * Cmn(", mc, ",", nc, ")")

for nc in range(0, rank, 4):
    for xc in range(0, rank, 4):
        for yc in range(0, rank, 4):
            print("\t\tGxy (", xc, ",", yc, ") = ", " Exn( ", xc, ",", nc, ") Bx (", nc, ", ", yc, ")")
            # print( "\t\tExn -> xc: ", xc, " nc: ", nc, "Bx -> ", " nc: ", nc ," yc: ", yc)


# for mc in range(0, rank, 4):
#     for nc in range(0, rank, 4):
#         print("Cmn -> mc: ", mc, " nc: ", nc)
#         for xc in range(0, rank, 4):
#             print("\tAx -> xc: ", xc, " mc: ", mc, " Cmn -> mc: ", mc, " nc: ", nc)
#             print("\t\tExn -> xc: ", xc, " nc: ", nc, " Bx -> nc: ", nc, " yc: ", nc)
            
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
def verify_multiply():
    block = 512
    a_v = np.random.randn(rank, block)
    b_u = np.random.randn(block, rank)
    a_s = np.random.randn(rank, rank)
    b_s = np.random.randn(rank, rank)

    dot_result = a_s.dot(a_v).dot(b_u).dot(b_s)

    Cmn = np.zeros((rank, rank))
    Exn = np.zeros((rank, rank))
    Gxy = np.zeros((rank, rank))

    print("===== VERIFY MULTIPLY ======")

    for mc in range(0, rank, 4):
        for nc in range(0, rank, 4):
            # Cmn[mc:mc+4, nc:nc+4] += a_v[mc:mc+4, :].dot(b_u[:, nc:nc+4])
            cmn = a_v[mc:mc+4, :].dot(b_u[:, nc:nc+4])
            print("Cmn( ", mc, ",", nc, ") = Av( ", mc, ",", ": ) * Bu(", " :, ", nc, " )")
            for xc in range(0, rank, 4):
                print("\tExn(", xc, ",", mc , ") = ", " Ax(", nc, "," , mc, ") * Cmn(", mc, ",", nc, ")")
                exn = a_s[xc:xc+4, mc:mc+4].dot(cmn)
                
                print("\t\tGxy (", xc, ",", nc, ") = ", " Exn( ", xc, ",", nc, ") Bx (", nc, ", ", mc, ")")
                Gxy[xc:xc+4,nc:nc+4] += exn.dot(b_s[nc:nc+4,mc:mc+4])

    print(Gxy)
    print(dot_result)

verify_multiply()

    



            

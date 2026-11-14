# trộm nhà
daypho = "8,3,5,3,6,7,8,9,0,0,1,2,7,3,6,3,8,43,6,8,2,1,1,1,3,4,2,1,3,4,2"
def trom_toi_da(nha):
    ndtt = 0   # tối đa tính đến nhà cách 2 bước phía trước
    ndtn = 0   # tối đa tính đến nhà ngay trước
    for tien in nha:
        ndtt, ndtn = ndtn, max(ndtn, ndtt + tien)
    return ndtn

tong_tien_trom_max = [int(x) for x in daypho.split(",")]
print(trom_toi_da(tong_tien_trom_max))
print("Số nhà:", len(tong_tien_trom_max))


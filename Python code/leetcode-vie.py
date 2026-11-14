# Bài trộm nhà (LeetCode 198 - House Robber)
# Không được trộm 2 nhà kề nhau. Tìm tổng tiền lớn nhất và các nhà cần trộm.
#11-11-2026
#

day_pho = "6,7,3,6,9,0,0,4,8,9,3,8,2,8,28,7,5,36,7,6,4,3,2,2,8,9,0,36,0,0,3,5,7,36,67,36"

def doc_day_pho(chuoi):  #cho mấy số này thành số nguyên không khoảng cách
    if chuoi.strip() == "":
        return None
    try:
        return [int(x) for x in chuoi.split(",")]
    except ValueError:
        return None

# ndtt = nhà định trộm trước
# ndtn = nhà định trộm này(hiện tại)
def trom_toi_da(nha):
    ndtt = 0   # tối đa tính đến nhà cách 2 bước phía trước
    ndtn = 0   # tối đa tính đến nhà ngay trước
    for tien in nha:
        ndtt, ndtn = ndtn, max(ndtn, ndtt + tien)
    return ndtn

#dc là địa chỉ nhà(index)
#i là số tiền tối đa có thể trộm được nếu dừng ở nhà dctt 
def chon_nha(nha):
    #Tìm các nhà được trộm (index từ 0): lưu bảng tiền tổng đã trộm được tính đến nhà đó rồi đi ngược từ cuối. O(n) bộ nhớ.
    n = len(nha)
    i = [0] * (n + 2)    # i[dc + 2]  tối đa tính đến nhà dctt; i[0], i[1] = 0 là "chưa có nhà"
    for dctt, tien in enumerate(nha):
        i[dctt + 2] = max(i[dctt + 1], i[dctt] + tien)
    chon = []
    dctt = n - 1
    while dctt >= 0:
        if i[dctt + 2] == i[dctt + 1]:   # không cần trộm nhà dctt
            dctt -= 1
        else:           # trộm địa chỉ đó (dctt), bỏ nhà dctt-1
            chon.append(dctt)
            dctt -= 2
    chon.reverse()
    return chon

#

# tong = tổng tiền tối đa trộm được
nha = doc_day_pho(day_pho)
if nha is None:
    print("Dãy nhà không trộm được, khóa cửa hết rồi.")
else: #dc = địa chỉ nhà(index)
    tong = trom_toi_da(nha)
    chon = chon_nha(nha)
    assert sum(nha[dc] for dc in chon) == tong   # kiểm tra chéo hai cách tính
    print("Số nhà:", len(nha))
    print("Tổng tiền tối đa:", tong)
    print("Các nhà cần trộm (index từ 0):", chon)
    print("Giá trị tiền bị trộm tương ứng:", [nha[dc] for dc in chon])
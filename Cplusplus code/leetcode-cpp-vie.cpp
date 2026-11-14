// Bài trộm nhà (LeetCode 198 - House Robber)
// Không được trộm 2 nhà kề nhau. Tìm tổng tiền lớn nhất và các nhà cần trộm.
//12-11-2026
// Biên dịch:    g++ -std=c++17 -O2 house_robber_vi.cpp -o house_robber_vi
// Cách chạy:    ./house_robber_vi
// Dãy riêng:    ./house_robber_vi "5,8,3,6"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

// Số tiền của từng nhà, cách nhau bằng dấu phẩy
const std::string day_pho =
    "6,7,3,6,9,0,36,4,8,9,3,8,2,8,6,7,35,36,37,6,4,3,2,2,8,9,0,36,0,0,3,5,7,36,67,36";

// Cắt chuỗi tại mỗi dấu phân cách. "6,7," cho ra {"6", "7", ""}: phần rỗng cuối vẫn được giữ.
std::vector<std::string> tach(const std::string& chuoi, char dau_tach) {
    std::vector<std::string> cac_phan;
    std::size_t dau = 0;
    while (true) {
        std::size_t cuoi = chuoi.find(dau_tach, dau);
        if (cuoi == std::string::npos) {
            cac_phan.push_back(chuoi.substr(dau));
            break;
        }
        cac_phan.push_back(chuoi.substr(dau, cuoi - dau));
        dau = cuoi + 1;
    }
    return cac_phan;
}

// Bỏ khoảng trắng ở hai đầu chuỗi.
std::string cat_khoang_trang(const std::string& chuoi) {
    const char* khoang_trang = " \t\r\n";
    std::size_t dau = chuoi.find_first_not_of(khoang_trang);
    if (dau == std::string::npos) return "";
    std::size_t cuoi = chuoi.find_last_not_of(khoang_trang);
    return chuoi.substr(dau, cuoi - dau + 1);
}

// Đổi chuỗi "9,4,4,..." thành các số. Trả về false nếu không hợp lệ.
bool doc_day_pho(const std::string& chuoi, std::vector<long long>& nha) {
    nha.clear();
    if (cat_khoang_trang(chuoi).empty()) return false;   // rỗng hoặc chỉ có dấu cách
    for (const std::string& tho : tach(chuoi, ',')) {
        std::string phan = cat_khoang_trang(tho);
        if (phan.empty()) return false;                  // dấu phẩy thừa như "6,7,"
        try {
            std::size_t da_doc = 0;
            long long gia_tri = std::stoll(phan, &da_doc);
            if (da_doc != phan.size()) return false;     // ví dụ "12abc"
            nha.push_back(gia_tri);
        } catch (const std::exception&) {                // không phải số, hoặc quá lớn
            return false;
        }
    }
    return true;
}

// Chỉ tính tổng tiền tối đa. O(n) thời gian, O(1) bộ nhớ.
long long trom_toi_da(const std::vector<long long>& nha) {
    long long toi_da_truoc2 = 0;   // tối đa tính đến nhà cách 2 bước phía trước
    long long toi_da_truoc1 = 0;   // tối đa tính đến nhà ngay trước
    for (long long tien : nha) {
        // C++ không có phép gán bộ như Python, nên giữ giá trị mới trong một biến trước.
        long long toi_da_nay = std::max(toi_da_truoc1, toi_da_truoc2 + tien);
        toi_da_truoc2 = toi_da_truoc1;
        toi_da_truoc1 = toi_da_nay;
    }
    return toi_da_truoc1;
}

// Trả về địa chỉ (index từ 0) các nhà cần trộm. O(n) thời gian, O(n) bộ nhớ.
//
// bang_toi_da[dc + 2] = số tiền tối đa nếu chỉ xét các nhà từ 0 đến dc
// (nhà dc có thể được trộm hoặc không).
// bang_toi_da[0] và bang_toi_da[1] bằng 0 nghĩa là "chưa có nhà nào",
// nhờ vậy hai nhà đầu không cần xử lý riêng.
std::vector<int> chon_nha(const std::vector<long long>& nha) {
    const int n = static_cast<int>(nha.size());
    std::vector<long long> bang_toi_da(n + 2, 0);
    for (int dc = 0; dc < n; ++dc) {
        // bỏ nhà dc:   bang_toi_da[dc + 1]
        // trộm nhà dc: tien + bang_toi_da[dc]  (nhà dc - 1 bị cấm)
        bang_toi_da[dc + 2] = std::max(bang_toi_da[dc + 1], bang_toi_da[dc] + nha[dc]);
    }

    // Đi ngược từ nhà cuối để tìm các nhà đã được trộm.
    std::vector<int> chon;
    int vi_tri = n - 1;
    while (vi_tri >= 0) {
        if (bang_toi_da[vi_tri + 2] == bang_toi_da[vi_tri + 1]) {
            vi_tri -= 1;                 // trộm nhà vi_tri không thêm được gì: bỏ qua
        } else {
            chon.push_back(vi_tri);      // trộm nhà vi_tri
            vi_tri -= 2;                 // nhà vi_tri - 1 kề nhau nên bị cấm, nhảy qua
        }
    }
    std::reverse(chon.begin(), chon.end());   // vì ta ghi từ cuối về đầu
    return chon;
}

// In nhãn rồi danh sách dạng [0, 2, 4].
template <typename T>
void in_danh_sach(const std::string& nhan, const std::vector<T>& gia_tri) {
    std::cout << nhan << "[";
    for (std::size_t k = 0; k < gia_tri.size(); ++k) {
        if (k > 0) std::cout << ", ";
        std::cout << gia_tri[k];
    }
    std::cout << "]\n";
}

int main(int argc, char* argv[]) {
    const std::string chuoi = (argc > 1) ? std::string(argv[1]) : day_pho;
    std::vector<long long> nha;
    if (!doc_day_pho(chuoi, nha)) {
        std::cout << "Dãy nhà không trộm được, khóa cửa hết rồi.\n";
        return 0;
    }

    const long long tong = trom_toi_da(nha);
    const std::vector<int> chon = chon_nha(nha);

    std::vector<long long> gia_tri;
    long long tong_chon = 0;
    for (int dc : chon) {
        gia_tri.push_back(nha[dc]);
        tong_chon += nha[dc];
    }
    if (tong_chon != tong) {   // kiểm tra chéo hai cách tính
        std::cerr << "Kiểm tra nội bộ thất bại: hai cách tính cho kết quả khác nhau.\n";
        return 1;
    }

    std::cout << "Số nhà: " << nha.size() << "\n";
    std::cout << "Tổng tiền tối đa: " << tong << "\n";
    in_danh_sach("Các nhà cần trộm (index từ 0): ", chon);
    in_danh_sach("Giá trị tương ứng: ", gia_tri);
    return 0;
}
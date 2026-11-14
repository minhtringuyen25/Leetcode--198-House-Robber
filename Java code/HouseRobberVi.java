// Bài trộm nhà (LeetCode 198 - House Robber)
// Không được trộm 2 nhà kề nhau. Tìm tổng tiền lớn nhất và các nhà cần trộm.
//
// Biên dịch:    javac -encoding UTF-8 HouseRobberVi.java
// Cách chạy:    java HouseRobberVi
// Dãy riêng:    java HouseRobberVi "5,8,2,9"

import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class HouseRobberVi {

    // Số tiền của từng nhà, cách nhau bằng dấu phẩy
    static final String DAY_PHO =
        "6,7,3,6,9,0,0,4,8,9,3,8,2,8,6,7,5,36,7,6,4,3,2,2,8,9,0,36,0,0,3,5,7,36,67,36";

    // Đổi chuỗi "9,4,4,..." thành danh sách số. Trả về null nếu không hợp lệ.
    static List<Long> docDayPho(String chuoi) {
        if (chuoi.trim().isEmpty()) {   // rỗng hoặc chỉ có dấu cách
            return null;
        }
        List<Long> nha = new ArrayList<>();
        // Số -1 giữ lại phần rỗng ở cuối, nên "6,7," bị từ chối thay vì bị bỏ qua âm thầm.
        for (String phan : chuoi.split(",", -1)) {
            try {
                nha.add(Long.parseLong(phan.trim()));
            } catch (NumberFormatException e) {   // không phải số, phần rỗng, hoặc quá lớn
                return null;
            }
        }
        return nha;
    }

    // Chỉ tính tổng tiền tối đa. O(n) thời gian, O(1) bộ nhớ.
    static long tromToiDa(List<Long> nha) {
        long toiDaTruoc2 = 0;   // tối đa tính đến nhà cách 2 bước phía trước
        long toiDaTruoc1 = 0;   // tối đa tính đến nhà ngay trước
        for (long tien : nha) {
            // Java không có phép gán bộ như Python, nên giữ giá trị mới trong một biến trước.
            long toiDaNay = Math.max(toiDaTruoc1, toiDaTruoc2 + tien);
            toiDaTruoc2 = toiDaTruoc1;
            toiDaTruoc1 = toiDaNay;
        }
        return toiDaTruoc1;
    }

    // Trả về địa chỉ (index từ 0) các nhà cần trộm. O(n) thời gian, O(n) bộ nhớ.
    //
    // bangToiDa[dc + 2] = số tiền tối đa nếu chỉ xét các nhà từ 0 đến dc
    // (nhà dc có thể được trộm hoặc không).
    // bangToiDa[0] và bangToiDa[1] bằng 0 nghĩa là "chưa có nhà nào",
    // nhờ vậy hai nhà đầu không cần xử lý riêng.
    static List<Integer> chonNha(List<Long> nha) {
        int n = nha.size();
        long[] bangToiDa = new long[n + 2];   // ban đầu toàn 0
        for (int dc = 0; dc < n; dc++) {
            // bỏ nhà dc:   bangToiDa[dc + 1]
            // trộm nhà dc: tien + bangToiDa[dc]  (nhà dc - 1 bị cấm)
            bangToiDa[dc + 2] = Math.max(bangToiDa[dc + 1], bangToiDa[dc] + nha.get(dc));
        }

        // Đi ngược từ nhà cuối để tìm các nhà đã được trộm.
        List<Integer> chon = new ArrayList<>();
        int viTri = n - 1;
        while (viTri >= 0) {
            if (bangToiDa[viTri + 2] == bangToiDa[viTri + 1]) {
                viTri -= 1;              // trộm nhà viTri không thêm được gì: bỏ qua
            } else {
                chon.add(viTri);         // trộm nhà viTri
                viTri -= 2;              // nhà viTri - 1 kề nhau nên bị cấm, nhảy qua
            }
        }
        Collections.reverse(chon);       // vì ta ghi từ cuối về đầu
        return chon;
    }

    public static void main(String[] args) throws UnsupportedEncodingException {
        // In ra UTF-8 để tiếng Việt hiển thị đúng dù hệ thống đặt mã hóa mặc định khác.
        PrintStream out = new PrintStream(System.out, true, "UTF-8");
        String chuoi = (args.length > 0) ? args[0] : DAY_PHO;
        List<Long> nha = docDayPho(chuoi);
        if (nha == null) {
            out.println("Dãy nhà không trộm được, khóa cửa hết rồi.");
            return;
        }

        long tong = tromToiDa(nha);
        List<Integer> chon = chonNha(nha);

        List<Long> giaTri = new ArrayList<>();
        long tongChon = 0;
        for (int dc : chon) {
            giaTri.add(nha.get(dc));
            tongChon += nha.get(dc);
        }
        // Kiểm tra chéo hai cách tính. (Lệnh "assert" của Java mặc định bị tắt nên dùng if.)
        if (tongChon != tong) {
            throw new IllegalStateException("Kiểm tra nội bộ thất bại: hai cách tính cho kết quả khác nhau.");
        }

        out.println("Số nhà: " + nha.size());
        out.println("Tổng tiền tối đa: " + tong);
        out.println("Các nhà cần trộm (index từ 0): " + chon);
        out.println("Giá trị tương ứng: " + giaTri);
    }
}
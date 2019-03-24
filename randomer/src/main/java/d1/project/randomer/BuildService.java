package d1.project.randomer;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.sql.SQLException;
import java.util.concurrent.ThreadLocalRandom;

@Service
public class BuildService {
    private final DBService dbService;
    private char[] current = new char[]{'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
    private char[] all = new char[]{
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
            'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
            'U', 'V', 'W', 'X', 'Y', 'Z'};

    @Autowired
    public BuildService(DBService dbService) {
        this.dbService = dbService;
    }

    void run() throws SQLException, ClassNotFoundException {
        long index = 0;
        long TOTAL = 1000000;
        //分组
        long PART = 100000;
        //分10个insert语句插入数据库
        for (long i = 0; i < TOTAL / PART; i++) {
            StringBuffer sb = new StringBuffer();
            dbService.initInsertSQL(sb);
            for (long j = i * PART; j < (i + 1) * PART; j++) {
                index++;
                if (index > TOTAL) break;
                dbService.appendInsertSQL(index, String.valueOf(current), sb);
                current = systemAdd(current);
            }
            sb.deleteCharAt(sb.length() - 1);
            dbService.excute(sb.toString());
        }
    }

    //36进制加一进位
    private char[] systemAdd(char[] ss) {
        int LENGTH = 10;
        for (int i = LENGTH - 1; i >= 0; i--) {
            int temp = ss[i];
            int number9 = '9';
            int numberZ = 'Z';
            if (temp < number9 - 1) {
                ss[i] = (char) ((int) ss[i] + 1);
                break;
            } else if (temp == number9) {
                ss[i] = 'A';
                break;
            } else if (temp < numberZ) {
                ss[i] = (char) ((int) ss[i] + 1);
                break;
            } else {
                ss[i] = '0';
            }
        }
        //100万前6位都是0，所以把前6位改成随机值
        for (int j = 0; j < 6; j++) {
            int temp = ThreadLocalRandom.current().nextInt(36);
            ss[j] = all[temp];
        }
        return ss;
    }
}

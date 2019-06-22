package d1.sample.gift;

import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * @author liuyi
 * 实际项目肯定是数据库的操作，2個表：用戶积分表和积分记录表，这里以2个list简单替代
 */
@Service
public class PointsService {
    private List<UserPoint> userPoints;
    private List<PointRecord> pointRecords;

    public PointsService() {
        userPoints = new ArrayList<>();
        pointRecords = new ArrayList<>();
        initUserPoint("wanger");
        initUserPoint("lisi");
    }

    public void printRecord() {
        for (UserPoint userPoint : userPoints) {
            System.out.println(userPoint.getUser() + "最后积分=" + userPoint.getPoint());
        }
        for (PointRecord pointRecord : pointRecords) {
            System.out.println(pointRecord.toString());
        }

    }

    public void addRulePoint(String rule, String user, Long point, Date date, String desc) {
        synchronized (this) {
            for (UserPoint userPoint : userPoints) {
                if (user != null && user.equals(userPoint.getUser())) {
                    userPoint.setPoint(userPoint.getPoint() + point);
                    PointRecord pointRecord = new PointRecord();
                    pointRecord.setUser(user);
                    pointRecord.setRule(rule);
                    pointRecord.setDate(date);
                    pointRecord.setPoint(point);
                    pointRecord.setDesc(desc);
                    pointRecords.add(pointRecord);
                }
            }
        }
    }

    public Long getPoint(String user) throws Exception {
        for (UserPoint userPoint : userPoints) {
            if (user != null && user.equals(userPoint.getUser())) {
                return userPoint.getPoint();
            }
        }
        throw new Exception("未找到用户:" + user);
    }

    private void initUserPoint(String name) {
        UserPoint userPoint = new UserPoint();
        userPoint.setUser(name);
        userPoint.setPoint(0L);
        userPoints.add(userPoint);
    }

    /**
     * 积分记录表
     */
    class PointRecord {
        /**
         * 用户名
         */
        private String user;
        /**
         * 增加积分数
         */
        private Long point;
        /**
         * 操作日期
         */
        private Date date;
        /**
         * 规则名称
         */
        private String rule;
        /**
         * 描述
         */
        private String desc;

        public String getUser() {
            return user;
        }

        public void setUser(String user) {
            this.user = user;
        }

        public Long getPoint() {
            return point;
        }

        public void setPoint(Long point) {
            this.point = point;
        }

        public Date getDate() {
            return date;
        }

        public void setDate(Date date) {
            this.date = date;
        }

        public String getRule() {
            return rule;
        }

        public void setRule(String rule) {
            this.rule = rule;
        }

        public String getDesc() {
            return desc;
        }

        public void setDesc(String desc) {
            this.desc = desc;
        }

        @Override
        public String toString() {
            return "PointRecord{" +
                    "user='" + user + '\'' +
                    ", point=" + point +
                    ", date=" + date +
                    ", rule='" + rule + '\'' +
                    ", desc='" + desc + '\'' +
                    '}';
        }
    }

    /**
     * 用户积分表
     */
    class UserPoint {
        /**
         * 用户名
         */
        private String user;
        /**
         * 用户当前积分
         */
        private Long point;

        public String getUser() {
            return user;
        }

        public void setUser(String user) {
            this.user = user;
        }

        public Long getPoint() {
            return point;
        }

        public void setPoint(Long point) {
            this.point = point;
        }
    }
}

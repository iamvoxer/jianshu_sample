package d1.project.randomer;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;

@Service
public class DBService {
    private static Logger logger = LoggerFactory.getLogger(DBService.class);
    @Value("${d1.project.randomer.dburl}")
    private String dbUrl;
    @Value("${d1.project.randomer.dbuser}")
    private String dbUser;
    @Value("${d1.project.randomer.dbpwd}")
    private String dbPwd;
    @Value("${d1.project.randomer.table}")
    private String tableName;

    public void initInsertSQL(StringBuffer sb) {
        sb.append("insert into ").append(tableName).append(" (random_index,random_value) values ");
    }

    public void appendInsertSQL(long index, String value, StringBuffer sb) {
        sb.append("(").append(index).append(",'").append(value).append("'),");
    }

    public void excute(String sql) throws SQLException, ClassNotFoundException {
        Connection dbConnection = null;
        try {
            Class.forName("com.mysql.jdbc.Driver");
            dbConnection = DriverManager.getConnection(dbUrl, dbUser, dbPwd);
            logger.info(dbUrl + dbUser + dbPwd + "连接成功");
            Statement st = dbConnection.createStatement();
            st.execute(sql);
            st.close();
        } catch (Exception e) {
            throw e;
        } finally {
            try {
                if (dbConnection != null)
                    dbConnection.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }
}

package d1.project.yjh;

import oracle.jdbc.pool.OracleDataSource;

import javax.sql.DataSource;
import java.math.BigDecimal;
import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class OracleConnector {
    private static DataSource dataSource() throws SQLException {

        OracleDataSource dataSource = new OracleDataSource();
        dataSource.setUser("user");
        dataSource.setPassword("password");
        dataSource.setURL("jdbc:oracle:thin:@localhost:1521:orcl");
        dataSource.setImplicitCachingEnabled(true);
        dataSource.setFastConnectionFailoverEnabled(true);
        return dataSource;
    }

    public static void connect() throws Exception {
        System.out.println("version1.4");
        DataSource dataSource = dataSource();

        try {
            Class.forName("org.postgresql.Driver");
            Connection db_pg = DriverManager.getConnection("jdbc:postgresql://ip地址:3432/dbname", "user", "password");
            Statement st_pg = db_pg.createStatement();

            Connection db = dataSource.getConnection();
            Statement st = db.createStatement();
            long before = System.currentTimeMillis();
            int count = 100000;
            StringBuffer sb = new StringBuffer(700 * count);

            String sql = "select * from interface_item_locator_history where rownum <" + count;
            ResultSet rs = getResultSet(st_pg, st, sql, sb);

            for (int i = 1; i < 60; i++) {
                sb = new StringBuffer(700 * count);
                sql = "SELECT *  FROM (SELECT a.*, ROWNUM rn  FROM interface_item_locator_history a)  WHERE rn >=" + (i * count) + "AND rn < " + ((i + 1) * count);
                rs = getResultSet(st_pg, st, sql, sb);
            }
            long after = System.currentTimeMillis();
            System.out.println("耗时:" + (after - before));
            st_pg.close();
            rs.close();
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private static ResultSet getResultSet(Statement st_pg, Statement st, String sql, StringBuffer sb) throws SQLException {
        sb.append("insert into pgbi_ods.interface_item_locator_history (item_id, item_type, org_id, item_org_id, item_class_id, inventory_id, manufactory_id, jyfs, po_tax_rate, so_tax_rate, cost_uc_ti, cost_uc_te, uc_ti, inv_quantity, inv_tc_ti, inv_tc_te, locator_id, inv_date, last_update_date, last_updated_by, creation_date, created_by) values ");
        ResultSet rs = st.executeQuery(sql);

        while (rs.next()) {
            interface_item_locator_history obj = new interface_item_locator_history();
            BigDecimal item_id = rs.getBigDecimal("item_id");
            obj.setItem_id(item_id);
            int item_type = rs.getInt("item_type");
            obj.setItem_type(item_type);
            BigDecimal org_id = rs.getBigDecimal("org_id");
            obj.setOrg_id(org_id);
            BigDecimal item_org_id = rs.getBigDecimal("item_org_id");
            obj.setItem_org_id(item_org_id);
            BigDecimal item_class_id = rs.getBigDecimal("item_class_id");
            obj.setItem_class_id(item_class_id);
            BigDecimal inventory_id = rs.getBigDecimal("inventory_id");
            obj.setInventory_id(inventory_id);
            BigDecimal manufactory_id = rs.getBigDecimal("manufactory_id");
            obj.setManufactory_id(manufactory_id);
            int jyfs = rs.getInt("jyfs");
            obj.setJyfs(jyfs);
            BigDecimal po_tax_rate = rs.getBigDecimal("po_tax_rate");
            obj.setPo_tax_rate(po_tax_rate);
            BigDecimal so_tax_rate = rs.getBigDecimal("so_tax_rate");
            obj.setSo_tax_rate(so_tax_rate);
            BigDecimal cost_uc_ti = rs.getBigDecimal("cost_uc_ti");
            obj.setCost_uc_ti(cost_uc_ti);
            BigDecimal cost_uc_te = rs.getBigDecimal("cost_uc_te");
            obj.setCost_uc_te(cost_uc_te);
            BigDecimal uc_ti = rs.getBigDecimal("uc_ti");
            obj.setUc_ti(uc_ti);
            BigDecimal inv_quantity = rs.getBigDecimal("inv_quantity");
            obj.setInv_quantity(inv_quantity);
            BigDecimal inv_tc_ti = rs.getBigDecimal("inv_tc_ti");
            obj.setInv_tc_ti(inv_tc_ti);
            BigDecimal inv_tc_te = rs.getBigDecimal("inv_tc_te");
            obj.setInv_tc_te(inv_tc_te);
            BigDecimal locator_id = rs.getBigDecimal("locator_id");
            obj.setLocator_id(locator_id);
            Date inv_date = rs.getDate("inv_date");
            obj.setInv_date(inv_date);
            Date last_update_date = rs.getDate("last_update_date");
            obj.setLast_update_date(last_update_date);
            String last_updated_by = rs.getString("last_updated_by");
            obj.setLast_updated_by(last_updated_by);
            Date creation_date = rs.getDate("creation_date");
            obj.setCreation_date(creation_date);
            String created_by = rs.getString("created_by");
            obj.setCreated_by(created_by);

            obj.toSQL(sb);
        }
        sb.deleteCharAt(sb.length() - 1);
        st_pg.execute(sb.toString());
        return rs;
    }
}

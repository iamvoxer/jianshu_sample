package d1.project.yjh;

import java.math.BigDecimal;
import java.sql.Date;

public class interface_item_locator_history {
    BigDecimal item_id;
    int item_type;
    BigDecimal org_id;
    BigDecimal item_org_id;
    BigDecimal item_class_id;
    BigDecimal inventory_id;
    BigDecimal manufactory_id;
    int jyfs;
    BigDecimal po_tax_rate;
    BigDecimal so_tax_rate;
    BigDecimal cost_uc_ti;
    BigDecimal cost_uc_te;
    BigDecimal uc_ti;
    BigDecimal inv_quantity;
    BigDecimal inv_tc_ti;
    BigDecimal inv_tc_te;
    BigDecimal locator_id;
    Date inv_date;
    Date last_update_date;
    String last_updated_by;
    Date creation_date;
    String created_by;

    /*
    insert into pgbi_ods.interface_item_locator_history
     (item_id, item_type, org_id, item_org_id, item_class_id, inventory_id, manufactory_id, jyfs, po_tax_rate,
     so_tax_rate, cost_uc_ti, cost_uc_te, uc_ti, inv_quantity, inv_tc_ti, inv_tc_te, locator_id, inv_date,
     last_update_date, last_updated_by, creation_date, created_by) values
      (201603160918310004405, 1, 70205105055219103497, 70205105055219103497, 70208121834438662177, 201801031108500000135,
      70208115357016077267, 9, 0.17, 0.17, 15.3616125, 13.12958333, 15.3616125, 169, 2596.11251184, 2218.89958277, 201801031108500000136,
      '2018-01-04', '2018-01-04', 'SYSTEM', '2018-01-04', 'SYSTEM')
     */
    public void toSQL(StringBuffer sb) {
        sb.append("(");
        sb.append(item_id).append(",");
        sb.append(item_type).append(",");
        sb.append(org_id).append(",");
        sb.append(item_org_id).append(",");
        sb.append(item_class_id).append(",");
        sb.append(inventory_id).append(",");
        sb.append(manufactory_id).append(",");
        sb.append(jyfs).append(",");
        sb.append(po_tax_rate).append(",");
        sb.append(so_tax_rate).append(",");
        sb.append(cost_uc_ti).append(",");
        sb.append(cost_uc_te).append(",");
        sb.append(uc_ti).append(",");
        sb.append(inv_quantity).append(",");
        sb.append(inv_tc_ti).append(",");
        sb.append(inv_tc_te).append(",");
        sb.append(locator_id).append(",");
        sb.append("'").append(inv_date).append("',");
        sb.append("'").append(last_update_date).append("',");
        sb.append("'").append(last_updated_by).append("',");
        sb.append("'").append(creation_date).append("',");
        sb.append("'").append(created_by).append("'),");
    }

    @Override
    public String toString() {
        return "{" +
                "item_id=" + item_id +
                ", item_type=" + item_type +
                ", org_id=" + org_id +
                ", item_org_id=" + item_org_id +
                ", item_class_id=" + item_class_id +
                ", inventory_id=" + inventory_id +
                ", manufactory_id=" + manufactory_id +
                ", jyfs=" + jyfs +
                ", po_tax_rate=" + po_tax_rate +
                ", so_tax_rate=" + so_tax_rate +
                ", cost_uc_ti=" + cost_uc_ti +
                ", cost_uc_te=" + cost_uc_te +
                ", uc_ti=" + uc_ti +
                ", inv_quantity=" + inv_quantity +
                ", inv_tc_ti=" + inv_tc_ti +
                ", inv_tc_te=" + inv_tc_te +
                ", locator_id=" + locator_id +
                ", inv_date=" + inv_date +
                ", last_update_date=" + last_update_date +
                ", last_updated_by='" + last_updated_by + '\'' +
                ", creation_date=" + creation_date +
                ", created_by='" + created_by + '\'' +
                '}' + "\n";
    }

    public BigDecimal getItem_id() {
        return item_id;
    }

    public void setItem_id(BigDecimal item_id) {
        this.item_id = item_id;
    }

    public int getItem_type() {
        return item_type;
    }

    public void setItem_type(int item_type) {
        this.item_type = item_type;
    }

    public BigDecimal getOrg_id() {
        return org_id;
    }

    public void setOrg_id(BigDecimal org_id) {
        this.org_id = org_id;
    }

    public BigDecimal getItem_org_id() {
        return item_org_id;
    }

    public void setItem_org_id(BigDecimal item_org_id) {
        this.item_org_id = item_org_id;
    }

    public BigDecimal getItem_class_id() {
        return item_class_id;
    }

    public void setItem_class_id(BigDecimal item_class_id) {
        this.item_class_id = item_class_id;
    }

    public BigDecimal getInventory_id() {
        return inventory_id;
    }

    public void setInventory_id(BigDecimal inventory_id) {
        this.inventory_id = inventory_id;
    }

    public BigDecimal getManufactory_id() {
        return manufactory_id;
    }

    public void setManufactory_id(BigDecimal manufactory_id) {
        this.manufactory_id = manufactory_id;
    }

    public int getJyfs() {
        return jyfs;
    }

    public void setJyfs(int jyfs) {
        this.jyfs = jyfs;
    }

    public BigDecimal getPo_tax_rate() {
        return po_tax_rate;
    }

    public void setPo_tax_rate(BigDecimal po_tax_rate) {
        this.po_tax_rate = po_tax_rate;
    }

    public BigDecimal getSo_tax_rate() {
        return so_tax_rate;
    }

    public void setSo_tax_rate(BigDecimal so_tax_rate) {
        this.so_tax_rate = so_tax_rate;
    }

    public BigDecimal getCost_uc_ti() {
        return cost_uc_ti;
    }

    public void setCost_uc_ti(BigDecimal cost_uc_ti) {
        this.cost_uc_ti = cost_uc_ti;
    }

    public BigDecimal getCost_uc_te() {
        return cost_uc_te;
    }

    public void setCost_uc_te(BigDecimal cost_uc_te) {
        this.cost_uc_te = cost_uc_te;
    }

    public BigDecimal getUc_ti() {
        return uc_ti;
    }

    public void setUc_ti(BigDecimal uc_ti) {
        this.uc_ti = uc_ti;
    }

    public BigDecimal getInv_quantity() {
        return inv_quantity;
    }

    public void setInv_quantity(BigDecimal inv_quantity) {
        this.inv_quantity = inv_quantity;
    }

    public BigDecimal getInv_tc_ti() {
        return inv_tc_ti;
    }

    public void setInv_tc_ti(BigDecimal inv_tc_ti) {
        this.inv_tc_ti = inv_tc_ti;
    }

    public BigDecimal getInv_tc_te() {
        return inv_tc_te;
    }

    public void setInv_tc_te(BigDecimal inv_tc_te) {
        this.inv_tc_te = inv_tc_te;
    }

    public BigDecimal getLocator_id() {
        return locator_id;
    }

    public void setLocator_id(BigDecimal locator_id) {
        this.locator_id = locator_id;
    }

    public Date getInv_date() {
        return inv_date;
    }

    public void setInv_date(Date inv_date) {
        this.inv_date = inv_date;
    }

    public Date getLast_update_date() {
        return last_update_date;
    }

    public void setLast_update_date(Date last_update_date) {
        this.last_update_date = last_update_date;
    }

    public String getLast_updated_by() {
        return last_updated_by;
    }

    public void setLast_updated_by(String last_updated_by) {
        this.last_updated_by = last_updated_by;
    }

    public Date getCreation_date() {
        return creation_date;
    }

    public void setCreation_date(Date creation_date) {
        this.creation_date = creation_date;
    }

    public String getCreated_by() {
        return created_by;
    }

    public void setCreated_by(String created_by) {
        this.created_by = created_by;
    }
}

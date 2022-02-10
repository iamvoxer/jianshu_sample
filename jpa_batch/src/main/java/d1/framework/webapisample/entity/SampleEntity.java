package d1.framework.webapisample.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * Entity示例
 * SampleEntity
 *
 * @author Buter
 * @date 2020/3/20 14:40
 */
@Entity
@Table(name = "d1_sample_entity")
public class SampleEntity {
    @Id
    @Column(columnDefinition = "varchar(100) COMMENT '主键id'")
    private String id;

    private Boolean booleanField;
    /**
     * 数字类型Field
     */
    private Integer intField;

    /**
     * 字符串类型Field
     */
    private String stringField;

    /**
     * 时间类型Field
     */
    private LocalDate dateField;
    /**
     * 时间类型Field
     */
    private LocalDate timeField;
    /**
     * 时间类型Field
     */
    private LocalDateTime dateTimeField;

    public Integer getIntField() {
        return intField;
    }

    public void setIntField(Integer intField) {
        this.intField = intField;
    }

    public String getStringField() {
        return stringField;
    }

    public void setStringField(String stringField) {
        this.stringField = stringField;
    }

    public LocalDate getDateField() {
        return dateField;
    }

    public void setDateField(LocalDate dateField) {
        this.dateField = dateField;
    }

    public LocalDate getTimeField() {
        return timeField;
    }

    public void setTimeField(LocalDate timeField) {
        this.timeField = timeField;
    }

    public LocalDateTime getDateTimeField() {
        return dateTimeField;
    }

    public void setDateTimeField(LocalDateTime dateTimeField) {
        this.dateTimeField = dateTimeField;
    }

    public Boolean getBooleanField() {
        return booleanField;
    }

    public void setBooleanField(Boolean booleanField) {
        this.booleanField = booleanField;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }
}

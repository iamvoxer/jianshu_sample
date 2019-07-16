package d1.sample.feign;

public class TestModel {
    String name;
    Integer sex;

    @Override
    public String toString() {
        return "TestModel{" +
                "name='" + name + '\'' +
                ", sex=" + sex +
                '}';
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getSex() {
        return sex;
    }

    public void setSex(Integer sex) {
        this.sex = sex;
    }
}

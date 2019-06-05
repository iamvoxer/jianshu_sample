public class Test1 {
    String name;
    Integer sex;

    @Override
    public String toString() {
        return "Test1{" +
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

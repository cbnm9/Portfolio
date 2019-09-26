package member;

public class Student extends Member {
	
	private String major;
	
	Student() {}
	public Student(int id, String name, String major)
	{
		super(id, name);
		this.major=major;
	}
	
	public String getMajor() {return major;}
	
	public void showData()
	{
		System.out.println(id + "\t" + name + "\t" + major);
	}
}

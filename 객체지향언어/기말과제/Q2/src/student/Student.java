package student;

public class Student {
	private int id;
	private String name;
	private String major;
	
	Student() {}
	public Student(int id, String name, String major)
	{
		this.id=id;
		this.name=name;
		this.major=major;
	}
	
	public int getID() {return id;}
	public String getName() {return name;}
	public String getMajor() {return major;}
	
	public void showData()
	{
		System.out.println(id + "\t" + name + "\t" + major);
	}
}

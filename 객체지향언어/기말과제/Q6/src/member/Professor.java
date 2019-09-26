package member;
import show.Showable;

public class Professor extends Member implements Showable {

	private String department;
	
	Professor() {}
	public Professor(int id, String name, String department)
	{
		super(id, name);
		this.department=department;
	}
	
	public String getDepartment() {return department;}
	
	public void showData()
	{
		System.out.println(id + "\t" + name + "\t" + department);
	}
}

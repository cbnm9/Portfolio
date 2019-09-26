package subject;
import show.Showable;

public class Subject implements Showable {
	private int id;
	private String title;
	
	Subject() {}
	public Subject(int id, String title)
	{
		this.id=id;
		this.title=title;
	}
	
	public int getID() {return id;}
	public String getTitle() {return title;}
	
	public void showData()
	{
		System.out.println(id + "\t" + title);
	}
}

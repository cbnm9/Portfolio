package member;

abstract public class Member {
	protected int id;
	protected String name;
	
	public Member() {}
	public Member(int id, String name)
	{
		this.id=id;
		this.name=name;
	}
	
	public int getID() {return id;}
	public String getName() {return name;}
	
	abstract public void showData();
}

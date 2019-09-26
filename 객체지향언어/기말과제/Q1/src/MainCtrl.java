
public class MainCtrl {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Student obj=new Student(1, "Young J. Rho", "Computer Engineering");
		
		System.out.println("---------------------------------------------");
		System.out.println("ID" + "\t" + "Name" + "\t\t" + "Major");
		System.out.println("---------------------------------------------");
		obj.ShowData();
		System.out.println("---------------------------------------------");
	}

}

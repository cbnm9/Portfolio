import member.MemberMgr;
import subject.SubjectMgr;
import classs.ClassMgr;

public class MainCtrl {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		MainMenu menu = new MainMenu();
		MemberMgr mm = new MemberMgr(50);
		SubjectMgr sm = new SubjectMgr(50);
		ClassMgr cm = new ClassMgr(50);
		
		mm.addDefaultMember();
		sm.addDefaultSubject();
		
		while(true)
		{
			int sel=menu.issueMenu();
			
			switch(sel)
			{
			case 1:
				mm.addStudent();
				break;
			case 2:
				mm.addProfessor();
				break;
			case 3:
				cm.addClass(sm, mm);
				break;
			case 4:
				mm.showAllStudent();
				break;
			case 5:
				mm.showAllProfessor();
				break;
			case 6:
				mm.showAllMember();
				break;
			case 7:
				sm.showAll();
				break;
			case 8:
				cm.showClass();
				break;
			case 9:
				cm.showAllClass();
				break;
			case 0:
				return;
			default:
				System.out.println("==> Wrong choice...");
				break;
			}
		}
	}

}

screenX = 77.7;
screenY = 55.5;
screenZ = 3.6;

frameX = screenX * 1.17;
frameY = screenY * 1.2;
frameZ = 4;

backHoleX = 50;
backHoleY = 30;
backHoleZ = 5;

bumpRadius = 2;

module screen() {
	cube([screenX, screenY, screenZ + 2]);
}

module frame() {
	
	difference() {
		translate([0, 0, 0]) cube([frameX, frameY, frameZ]);
		translate([(frameX - screenX) / 2, (frameY - screenY) / 2, frameZ - screenZ]) screen();
		
		// Interface for left tabs
		translate([((frameX - screenX) / 2) + 2, ((frameY - screenY) / 2) - 1.5, frameZ - screenZ]) cube([1.5, 1.5, screenZ + 1]);
		translate([(frameX - screenX) / 2, ((frameY - screenY) / 2) - 1.5, frameZ - screenZ]) cube([3, 2, screenZ/2]);
		translate([((frameX - screenX) / 2) + 2, ((frameY - screenY) / 2) + screenY, frameZ - screenZ]) cube([1.5, 1.5, screenZ + 1]);
		translate([(frameX - screenX) / 2, ((frameY - screenY) / 2) + screenY, frameZ - screenZ]) cube([3, 2, screenZ/2]);

		// Top tab
		translate([((frameX - screenX) / 2) + 28.8, ((frameY - screenY) / 2) + screenY, frameZ - screenZ]) cube([3.5, 1.5, screenZ + 1]);
		// Bottom tabs
		translate([((frameX - screenX) / 2) + 20, ((frameY - screenY) / 2) - 1.1, frameZ - screenZ]) cube([3.5, 1.5, screenZ + 1]);
		translate([((frameX - screenX) / 2) + 55, ((frameY - screenY) / 2) - 1.1, frameZ - screenZ]) cube([3.5, 1.5, screenZ + 1]);
		
		// Right tabs
		translate([(frameX - (frameX - screenX) / 2), ((frameY - screenY) / 2), frameZ - screenZ]) cube([1.5, 3.5, screenZ + 1]);
		//translate([(frameX - (frameX - screenX) / 2) - 1, ((frameY - screenY) / 2), frameZ - screenZ]) cube([1.5, 3.5, screenZ + 1]);
		
		translate([(frameX - (frameX - screenX) / 2), frameY - ((frameY - screenY) / 2) - 3.5, frameZ - screenZ]) cube([1.5, 3.5, screenZ + 1]);
		//translate([(frameX - (frameX - screenX) / 2) - 1, frameY - ((frameY - screenY) / 2) - 3.5, frameZ - screenZ]) cube([1.5, 3.5, screenZ + 1]);
	
	

		translate([(frameX - screenX) / 2, (frameY / 2) - (backHoleY / 2), -backHoleZ / 2]) cube([backHoleX, backHoleY, backHoleZ]);
		translate([backHoleX * .65, (frameY / 2) - (backHoleY / 2), -backHoleZ/2]) cube([backHoleX / 2, backHoleY, backHoleZ * 2]);

	}

/*
	translate([((frameX - screenX) / 2) + 15, ((frameY - screenY) / 2) - (bumpRadius / 2), 0]) cylinder(h=frameZ, r=bumpRadius, $fn=20);
	translate([((frameX - screenX) / 2) + 15, frameY - ((frameY - screenY) / 2) + (bumpRadius / 2) , 0]) cylinder(h=frameZ, r=bumpRadius, $fn=20);
	translate([frameX - ((frameX - screenX) / 2) - 15, ((frameY - screenY) / 2) - (bumpRadius / 2), 0]) cylinder(h=frameZ, r=bumpRadius, $fn=20);
	translate([frameX - ((frameX - screenX) / 2) - 15, frameY - ((frameY - screenY) / 2) + (bumpRadius / 2) , 0]) cylinder(h=frameZ, r=bumpRadius, $fn=20);
*/

}


frame();


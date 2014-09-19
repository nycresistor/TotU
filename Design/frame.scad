screenX = 77.7;
screenY = 55.04;
screenZ = 3.6;

frameX = screenX * 1.17;
frameY = screenY * 1.2;
frameZ = 7;

backHoleX = 50;
backHoleY = 30;
backHoleZ = 5;

bumpRadius = 2;

module screen() {
	cube([screenX, screenY, screenZ + 2]);
}

module frame() {
	
	difference() {
		cube([frameX, frameY, frameZ]);
		translate([(frameX - screenX) / 2, (frameY - screenY) / 2, frameZ - screenZ]) screen();
		translate([(frameX - screenX) / 2, (frameY / 2) - (backHoleY / 2), -backHoleZ / 2]) cube([backHoleX, backHoleY, backHoleZ]);
		translate([backHoleX * .65, (frameY / 2) - (backHoleY / 2), -backHoleZ/2]) cube([backHoleX / 2, backHoleY, backHoleZ * 2]);

	}

	translate([((frameX - screenX) / 2) + 15, ((frameY - screenY) / 2) - (bumpRadius / 2), 0]) cylinder(h=frameZ, r=bumpRadius, $fn=20);
	translate([((frameX - screenX) / 2) + 15, frameY - ((frameY - screenY) / 2) + (bumpRadius / 2) , 0]) cylinder(h=frameZ, r=bumpRadius, $fn=20);
	translate([frameX - ((frameX - screenX) / 2) - 15, ((frameY - screenY) / 2) - (bumpRadius / 2), 0]) cylinder(h=frameZ, r=bumpRadius, $fn=20);
	translate([frameX - ((frameX - screenX) / 2) - 15, frameY - ((frameY - screenY) / 2) + (bumpRadius / 2) , 0]) cylinder(h=frameZ, r=bumpRadius, $fn=20);

}


frame();


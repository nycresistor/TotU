screenX = 78.2;
screenY = 55.75;
screenZ = 3.6;

frameX = screenX * 1.16;
frameY = screenY * 1.2;
frameZ = 4;

backHoleX = 40;
backHoleY = 30;
backHoleZ = 5;

bumpRadius = 2;

magnetX = 9.525;
magnetY = 19.05;
magnetZ = 3.175;

lensX = frameX + 9.525 / 2;
lensY = frameY * 1.05;
lensZ = frameZ * 1.2;

activeX = 64.8;
activeY = 48.6;
activeZ = 10;

mountOuterDia = 12.7;
mountInnerDia = 7;
mountOffset = 8;


module screen() {
	cube([screenX, screenY, screenZ + 2]);
}

module magnet() {
	translate([0, 0, 0]) cube([magnetX, magnetY, magnetZ + 2]);
}

module mount() {
	difference() {
		union() {
			translate([-mountOuterDia/2,0,0]) cube([mountOuterDia, mountOffset, frameZ]);
			cylinder(h=frameZ, d=mountOuterDia);
		}
		translate([0,0,-0.1]) cylinder(h=frameZ+0.2, d=mountInnerDia);
	}
}

module frame() {
	
	difference() {
		
		// Frame and screen
		translate([0, 0, 0]) cube([frameX, frameY, frameZ]);
		translate([(frameX - screenX) / 2, (frameY - screenY) / 2, frameZ - screenZ]) screen();
		
		// Interface for left tabs
		translate([((frameX - screenX) / 2) + 2, ((frameY - screenY) / 2) - 1.5, frameZ - screenZ]) cube([1.5, 1.5, screenZ + 1]);
		translate([(frameX - screenX) / 2, ((frameY - screenY) / 2) - 1.5, frameZ - screenZ]) cube([3, 2, screenZ/2]);
		translate([((frameX - screenX) / 2) + 2, ((frameY - screenY) / 2) + screenY, frameZ - screenZ]) cube([1.5, 1.5, screenZ + 1]);
		translate([(frameX - screenX) / 2, ((frameY - screenY) / 2) + screenY, frameZ - screenZ]) cube([3, 2, screenZ/2]);
		translate([((frameX - screenX) / 2)-1.5, ((frameY - screenY) / 2), frameZ - screenZ]) cube([1.6, 5.5, screenZ + 1]);		
		translate([((frameX - screenX) / 2)-1.5, frameY - ((frameY - screenY) / 2) - 5.5, frameZ - screenZ]) cube([1.6, 5.5, screenZ + 1]);



		// Top tab
		translate([((frameX - screenX) / 2) + 28.8, ((frameY - screenY) / 2) + screenY, frameZ - screenZ]) cube([3.5, 1.5, screenZ + 1]);
		
		// Bottom tabs
		translate([((frameX - screenX) / 2) + 20, ((frameY - screenY) / 2) - 1.1, frameZ - screenZ]) cube([3.5, 1.5, screenZ + 1]);
		#translate([((frameX - screenX) / 2) + 33.25, ((frameY - screenY) / 2) - 1.1, frameZ - screenZ]) cube([3.5, 1.5, screenZ + 1]);
		translate([((frameX - screenX) / 2) + 55, ((frameY - screenY) / 2) - 1.1, frameZ - screenZ]) cube([3.5, 1.5, screenZ + 1]);
		
		// Right tabs
		translate([(frameX - (frameX - screenX) / 2), ((frameY - screenY) / 2), frameZ - screenZ]) cube([1.5, 5.5, screenZ + 1]);		
		translate([(frameX - (frameX - screenX) / 2), frameY - ((frameY - screenY) / 2) - 5.5, frameZ - screenZ]) cube([1.5, 5.5, screenZ + 1]);
	
		// Back hole
		translate([(frameX - screenX) / 2, (frameY / 2) - (backHoleY / 2), -backHoleZ / 2]) cube([backHoleX, backHoleY, backHoleZ]);
		translate([backHoleX * .65, (frameY / 2) - (backHoleY / 2), -backHoleZ/2]) cube([backHoleX / 2, backHoleY, backHoleZ * 2]);
		
		translate([(frameX - 25), frameY / 2, -5]) cylinder(h=10, r=10);
		
		// Magnet holes
		//translate([-magnetX / 1.5, (frameY / 2) - (magnetY / 2), -1]) magnet();
		//translate([frameX - (magnetX / 3), (frameY / 2) - (magnetY / 2), -1]) magnet();
	}

}

module lens() {
	difference() {
		// Lens block
		translate([5.5, -0.5, 0]) translate([-magnetX, -(lensY - frameY) / 2, 0]) cube([lensX, lensY, lensZ]);
		
	translate([-1, 0, 0]) {
		// Opening
		translate([(frameX - screenX) / 2, (frameY - screenY) / 2, -activeZ / 3]) {
			translate([8, (screenY - activeY) / 2, 0]) cube([activeX, activeY, activeZ]);
		}
		
		translate([-1, -1, -2]) cube([frameX + 0.5, frameY + 0.5, frameZ + 2]);

		// Magnet holes
		//translate([-magnetX / 1.5, (frameY / 2) - (magnetY / 2), -1]) magnet();
		//translate([frameX - (magnetX / 3), (frameY / 2) - (magnetY / 2), -1]) magnet();

		}
		
	}
}

module frameWithMount() {
	union() {
		frame();
		translate([frameX+mountOffset-0.1, frameY-6.85, 0]) rotate([0,0,90]) mount();
		translate([-mountOffset+0.1, frameY-6.85, 0]) rotate([0,0,270]) mount();
	}
}

module lensMinusMount() {
	difference() {
		lens();
		translate([frameX-5,frameY-14.75,-0.25]) cube([mountOuterDia, mountOuterDia+2, frameZ+0.5]);
		translate([-9,frameY-14.75,-0.25]) cube([mountOuterDia, mountOuterDia+2, frameZ+0.5]);
	}
}


frameWithMount();
//lensMinusMount();



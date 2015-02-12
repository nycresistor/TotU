var TOTU = TOTU || {};

TOTU.Screen = function Screen(parent, n) {
	var self = this;

	this.subno = Math.floor(Math.random()*512);
	this.parent = parent;
	this.n = 0;

	this.init = function init() {
		this.setSize();
	}

	this.setSize = function setSize() {
		var config = TOTU.exploreConfig;

		this.width = config.screen.width / config.scaledown;
		this.height = config.screen.height / config.scaledown;
		this.y = Math.floor(n / 4) * this.height + 
			config.screen.paddingTop / config.scaledown * (Math.floor(n / 4) + 1) +
			config.panel.paddingTop / config.scaledown;

		this.x = ((n % 4) * config.screen.width + 
			config.screen.paddingLeft * ((n % 4 )+ 1) +
			config.panel.paddingLeft) / config.scaledown;
	}

	this.paint = function paint() {
		var submissions = TOTU.explore.submissions;

		if (submissions.length) {
			var sub = self.subno % submissions.length;
			var photos = submissions[sub].photos;
				self.parent.ctx.drawImage(photos[self.n], self.x, self.y, self.width, self.height);
			self.n++;
			if (self.n>=photos.length) self.n=0;
		}
		else {
			self.parent.ctx.fillStyle = "black";
			self.parent.ctx.fillRect(self.x, self.y, self.width, self.height);
		}

		return true;
	}

	this.updateResolution = function updateResolution() {
		self.setSize();
	}

	this.init();
}

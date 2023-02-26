#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(1);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(300, 7);
	this->triangle_list.insert(this->triangle_list.end(), ico_sphere.getMesh().getUniqueFaces().begin(), ico_sphere.getMesh().getUniqueFaces().end());

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(1000);

	this->mesh.clear();
	this->frame.clear();

	for (float radius = 150; radius <= 250; radius += 100) {

		auto noise_seed = ofRandom(1000);
		for (int i = 0; i < this->triangle_list.size(); i++) {

			glm::vec3 avg = (this->triangle_list[i].getVertex(0) + this->triangle_list[i].getVertex(1) + this->triangle_list[i].getVertex(2)) / 3;
			auto noise_value = ofNoise(noise_seed, avg.x * 0.0025, avg.y * 0.0025 + ofGetFrameNum() * 0.035, avg.z * 0.0025);
			if (noise_value < 0.47 || noise_value > 0.52) { continue; }

			vector<glm::vec3> vertices;

			vertices.push_back(glm::normalize(this->triangle_list[i].getVertex(0)) * (radius + 15) - avg);
			vertices.push_back(glm::normalize(this->triangle_list[i].getVertex(1)) * (radius + 15) - avg);
			vertices.push_back(glm::normalize(this->triangle_list[i].getVertex(2)) * (radius + 15) - avg);

			vertices.push_back(glm::normalize(this->triangle_list[i].getVertex(0)) * (radius - 15) - avg);
			vertices.push_back(glm::normalize(this->triangle_list[i].getVertex(1)) * (radius - 15) - avg);
			vertices.push_back(glm::normalize(this->triangle_list[i].getVertex(2)) * (radius - 15) - avg);

			for (auto& vertex : vertices) {

				vertex += avg;
			}

			this->mesh.addVertices(vertices);
			this->frame.addVertices(vertices);

			for (int k = 0; k < vertices.size(); k++) {

				if (radius == 150) {

					this->mesh.addColor(ofColor(192, 0, 0));
					this->frame.addColor(ofColor(255, 0, 0));
				}
				else {

					this->mesh.addColor(ofColor(50, 50, 192));
					this->frame.addColor(ofColor(100, 100, 255));
				}
			}

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 3);
			this->mesh.addTriangle(this->mesh.getNumVertices() - 4, this->mesh.getNumVertices() - 5, this->mesh.getNumVertices() - 6);

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 5);
			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 5, this->mesh.getNumVertices() - 4);

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 3, this->mesh.getNumVertices() - 6);
			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 6, this->mesh.getNumVertices() - 4);

			this->mesh.addTriangle(this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 3, this->mesh.getNumVertices() - 6);
			this->mesh.addTriangle(this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 6, this->mesh.getNumVertices() - 5);

			this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 2);
			this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 3);
			this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 3);

			this->frame.addIndex(this->frame.getNumVertices() - 4); this->frame.addIndex(this->frame.getNumVertices() - 5);
			this->frame.addIndex(this->frame.getNumVertices() - 5); this->frame.addIndex(this->frame.getNumVertices() - 6);
			this->frame.addIndex(this->frame.getNumVertices() - 4); this->frame.addIndex(this->frame.getNumVertices() - 6);

			this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 4);
			this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 5);
			this->frame.addIndex(this->frame.getNumVertices() - 3); this->frame.addIndex(this->frame.getNumVertices() - 6);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum());

	this->mesh.drawFaces();
	this->frame.drawWireframe();

	this->cam.end();

	ostringstream os;
	os << setw(4) << setfill('0') << ofGetFrameNum();
	ofImage image;
	image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	image.saveImage("image/img_" + os.str() + ".jpg");
	if (ofGetFrameNum() >= 30 * 20 + 1) {

		std::exit(1);
	}

	// > ffmpeg -i img_%04d.jpg -r 30 out.mp4
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
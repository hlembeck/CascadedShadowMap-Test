#include "Scene.h"
#include "StaticShapes.h"

Scene::~Scene() {
	for (auto i = m_pObjectsInstances.begin(); i != m_pObjectsInstances.end(); i++) {
		delete[] i->pObject;
	}
}

void Scene::Load() {
	ObjectInstances objInstances;
	StaticCube* cube = new StaticCube(SIMPLE);

	cube->LoadObject(m_device.Get());
	cube->LoadRandomInstances(m_device.Get(), 1, (RandomGenerator*)this, 0.0f);

	objInstances.pObject = cube;
	objInstances.nInstances = 1;

	m_pObjectsInstances.push_back(objInstances);
}

void Scene::DrawDepth(ID3D12GraphicsCommandList* commandList) {

}

void Scene::DrawFinal(ID3D12GraphicsCommandList* commandList) {
	VertexView currView;
	for (auto i = m_pObjectsInstances.begin(); i != m_pObjectsInstances.end(); i++) { //Inefficient -- lots of draws possible
		currView = i->pObject->m_vertexView;
		commandList->IASetVertexBuffers(0, 1, &currView.view);
		commandList->SetPipelineState(m_simplePSO.Get());
		commandList->DrawInstanced(currView.nVertices,i->nInstances,0,0);
	}
}


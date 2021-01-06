#pragma once
#include <string>
#include "vector2.h"

namespace EcoSim
{
	class CellMatrix;

	/// <summary>
	/// Cell 中包含的内容，一个生物。
	/// </summary>
	class ILivingThing
	{
	public:
		virtual ~ILivingThing() {}
		/// <summary>
		/// 移动阶段，决定移动的目的地。移动到非空格子内表示捕食。
		/// </summary>
		/// <param name="map">地图</param>
		/// <param name="pos">此格位置</param>
		/// <returns></returns>
		auto virtual DecideDestination(const CellMatrix& map, Vector2 pos)->Vector2 = 0;

		/// <summary>
		/// 繁殖阶段，决定在哪个位置生成子代。 
		/// </summary>
		/// <param name="map">地图</param>
		/// <param name="pos">此格位置</param>
		/// <returns></returns>
		auto virtual DecideChildrenLocation(const CellMatrix& map, Vector2 pos)->std::vector<Vector2> = 0;

		/// <summary>
		/// 繁殖阶段，构造子代对象。
		/// </summary>
		/// <param name="map">地图</param>
		/// <param name="pos">此格位置</param>
		/// <returns></returns>
		auto virtual Reproduce()->std::shared_ptr<ILivingThing> = 0;

		/// <summary>
		/// 显示颜色。
		/// </summary>
		/// <returns></returns>
		auto virtual DisplayColor() const-> int = 0;

		/// <summary>
		/// 返回类型名称。
		/// </summary>
		/// <returns></returns>
		auto virtual TypeIdentifier() const->std::string = 0;
		 
		auto virtual Birth() const -> void = 0;

		auto virtual Die() const -> void = 0;
	};
}
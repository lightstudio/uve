using System;
using System.Collections.Generic;

namespace UVEngine
{
    ///// <summary>
    ///// 为应用程序实现 IServiceProvider。此类型通过 App.Services
    ///// 属性公开，可用于 ContentManagers 或其他需要访问 IServiceProvider 的类型。
    ///// </summary>
    //public class AppServiceProvider : IServiceProvider
    //{
    //    // 服务本身的服务类型映射
    //    private readonly Dictionary<Type, object> services = new Dictionary<Type, object>();

    //    /// <summary>
    //    /// 将新服务添加到服务提供程序。
    //    /// </summary>
    //    /// &lt;param name="serviceType"&gt;要添加的服务类型。</param>
    //    /// &lt;param name="service"&gt;服务对象本身。</param>
    //    public void AddService(Type serviceType, object service)
    //    {
    //        // 验证输入
    //        if (serviceType == null)
    //            throw new ArgumentNullException("serviceType");
    //        if (service == null)
    //            throw new ArgumentNullException("service");
    //        if (!serviceType.IsAssignableFrom(service.GetType()))
    //            throw new ArgumentException("service does not match the specified serviceType");

    //        // 将服务添加到词典
    //        services.Add(serviceType, service);
    //    }

    //    /// <summary>
    //    /// 从服务提供程序获取服务。
    //    /// </summary>
    //    /// &lt;param name="serviceType"&gt;要检索的服务类型。</param>
    //    /// <returns>为指定类型注册的服务对象。</returns>
    //    public object GetService(Type serviceType)
    //    {
    //        // 验证输入
    //        if (serviceType == null)
    //            throw new ArgumentNullException("serviceType");

    //        // 从字典中检索服务
    //        return services[serviceType];
    //    }

    //    /// <summary>
    //    /// 从服务提供程序删除服务。
    //    /// </summary>
    //    /// &lt;param name="serviceType"&gt;要删除的服务类型。</param>
    //    public void RemoveService(Type serviceType)
    //    {
    //        // 验证输入
    //        if (serviceType == null)
    //            throw new ArgumentNullException("serviceType");

    //        // 从词典中删除服务
    //        services.Remove(serviceType);
    //    }
    //}
}

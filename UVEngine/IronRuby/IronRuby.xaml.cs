//using System;
//using System.IO;
//using System.Collections.Generic;
//using System.Linq;
//using System.Net;
//using System.Windows;
//using System.Windows.Controls;
//using System.Windows.Documents;
//using System.Windows.Input;
//using System.Windows.Media;
//using System.Windows.Media.Animation;
//using System.Windows.Shapes;
//using Microsoft.Phone.Controls;
//using System.Resources;
//using UVE_IronRuby;
//using System.Reflection;
//using UVE_Media;
//using Microsoft.Xna.Framework;

//namespace UVEngine.AlphaLang
//{
//    public partial class IronRuby : PhoneApplicationPage
//    {
//        UVE_IronRubyEngine irEngine = new UVE_IronRubyEngine();
//        public PropertyPath GetPropertyPath(string str)
//        {
//            return new PropertyPath(str);
//        }

//        public IronRuby()
//        {
//            InitializeComponent();
//            // Allow both portrait and landscape orientations
//            SupportedOrientations = SupportedPageOrientation.PortraitOrLandscape;
//            // Create an IronRuby engine and prevent compilation
//            //ScriptEngine engine = Ruby.CreateEngine();
//            //Storyboard story = new Storyboard();
//            //DoubleAnimation animation;
//            //animation = new DoubleAnimation();
//            //animation.From = 0;
//            //animation.To = 1;
//            //animation.Duration = new Duration(TimeSpan.FromMilliseconds(5000));
//            //Storyboard.SetTarget(animation, LayoutRoot);
//            //Storyboard.SetTargetProperty(animation, new PropertyPath("(UIElement.Opacity)"));
//            //story.Children.Add(animation);

//            //story.AutoReverse = true;
//            //story.RepeatBehavior = RepeatBehavior.Forever;

//            //story.Begin();


//            // Load the System.Windows.Media assembly to the IronRuby context
//            irEngine.LoadAssembly(typeof(System.Windows.Media.Color).Assembly);
            
//            irEngine.LoadAssembly(typeof(UVE_Audio).Assembly);
//            irEngine.LoadAssembly(typeof(UVE_Image).Assembly);
//            //irEngine.LoadAssembly(typeof(UVEngineNative.NativeScript).Assembly);
//            irEngine.LoadAssembly(typeof(UVEngineNative.UVEDelegate).Assembly);
//            irEngine.LoadAssembly(typeof(UVEngineNative.GameInfoEx).Assembly);
//            irEngine.LoadAssembly(typeof(UVEngine.UVECallBack).Assembly);
//            // Add a global constant named Phone, which will allow access to this class
//            irEngine.SetVariable("Phone", this);
//            //irEngine.SetVariable("Audio", audio);
            
//            // Read the IronRuby code
//            Assembly execAssembly = Assembly.GetExecutingAssembly();
//            irEngine.LoadAssembly(execAssembly);
//            string code = new StreamReader(Application.GetResourceStream(new Uri("IronRuby\\Main.rb", UriKind.Relative)).Stream).ReadToEnd();

//            // Execute the IronRuby code
//            this.Dispatcher.BeginInvoke(() =>
//            {
//                try
//                {
//                    irEngine.Execute(code);
//                }
//                catch (Exception e) { MessageBox.Show(e.Message + '\n' + e.StackTrace); }
//            });
//        }
//        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
//        {
//            base.OnBackKeyPress(e);
//            //irEngine.Execute("Back");
//        }
//    }
//}
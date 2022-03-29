import axios from 'axios';
import { useState, useEffect } from 'react';
import CartList from './Components/CartList';
import HomeWrapper from './Components/HomeWrapper';
import Loading from './Components/Loading';
import SectionWrapper from './Components/SectionWrapper';
import ShopList from './Components/ShopList';

function App() {
  const [loading, setLoading] = useState(true);
  const [cart, setCart] = useState([]);
  const [items, setItems] = useState([]);

  const onAddItem = (item) => {
    setItems(items.filter((it) => it.id !== item.id));
    setCart([...cart, { ...item, quantity: 1, basePrice: item.price }]);
  };

  const onRemoveCartItem = (item) => {
    setCart(cart.filter((it) => it.id !== item.id));
    setItems([...items, { ...item, price: item.basePrice }]);
  };

  const onUpdateCartItemQuantity = (item, value) => {
    const newCart = [...cart];
    const itemIndex = cart.findIndex((r) => r.id === item.id);
    newCart[itemIndex] = {
      ...item,
      quantity: value,
      price: item.basePrice * value,
    };
    setCart(newCart);
  };

  const onBuyItems = () => {
    setCart(cart.map((it) => ({ ...it, bought: true })));
    console.log(cart);
  };

  useEffect(() => {
    const fetchProducts = async () => {
      setLoading(true);
      const { data } = await axios.get('https://fakestoreapi.com/products');
      setItems(data);
      setLoading(false);
    };

    fetchProducts();
  }, []);

  return (
    <HomeWrapper>
      <SectionWrapper label="Shop">
        {loading && <Loading />}
        <ShopList items={items} addToCart={(item) => onAddItem(item)} />
      </SectionWrapper>
      <SectionWrapper label="Cart">
        <CartList
          items={cart}
          onUpdateItemQuantity={(item, value) => {
            onUpdateCartItemQuantity(item, value);
          }}
          onRemoveItem={(item) => {
            onRemoveCartItem(item);
          }}
        />
        {cart.length > 0 && (
          <button
            onClick={() => onBuyItems()}
            className="px-10 py-4 bg-gray-800 text-white text-lg font-bold uppercase rounded"
          >
            BUY
          </button>
        )}
      </SectionWrapper>
    </HomeWrapper>
  );
}

export default App;
